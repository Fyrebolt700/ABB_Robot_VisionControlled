#include <librealsense2/rs.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void sendTriggerSignal() {
    WSADATA wsaData;
    SOCKET SendSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }
    // Create a socket for sending data (TCP)
    SendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SendSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return;
    }

    // Set up sockaddr_in with connection info
	clientService.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1", &clientService.sin_addr);
	clientService.sin_port = htons(55000);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Connect to the server (AKA robot studio)
    if (connect(SendSocket, (struct sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        std::cerr << "Connect failed." << std::endl;
        closesocket(SendSocket);
        WSACleanup();
        return;
	}

	//message sent to robot studio
	const char* message = "GO";
	send(SendSocket, message, (int)strlen(message), 0);

	//close the socket and clean up
	closesocket(SendSocket);
	WSACleanup();
}

int main() {
	// print a welcome message (troubleshooting)
    std::cout << "Program started\n";

	// configure and start the pipeline between the application and the camera
    rs2::pipeline p;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    p.start(cfg);

	// get the depth sensor's information (troubleshooting)
    auto profile = p.get_active_profile();
    auto device = profile.get_device();
    std::cout << "Device Name: " << device.get_info(RS2_CAMERA_INFO_NAME) << std::endl;

	// 0.5m distance threshold for triggering an action
    const float TRIGGER_DISTANCE = 1.0f;
    // tolerance for distance measurement
	const float TOLERANCE = 0.2f; 
	//variable to store if object is present
    bool object_present = false;

    while (true) {
		// wait for the next set of frames from the camera
		// make sure the frameset contains a depth frame (troubleshoting)
        rs2::frameset frames = p.wait_for_frames();
        if (!frames) {
            std::cout << "No frames received." << std::endl;
            continue;
        }

		// get the depth frame
		// ensure the depth frame is valid
        rs2::depth_frame depth = frames.get_depth_frame();
        if (!depth) {
            std::cerr << "No depth frame received!" << std::endl;
            continue;
        }

		// get the distance from the camera to the object in the center of the image
        float width = depth.get_width();
        float height = depth.get_height();
        float dist_to_center = depth.get_distance(width / 2, height / 2);

        //std::cout << "Depth frame size: " << width << " x " << height << std::endl;
        std::cout << "Distance to camera: " << dist_to_center << " meters\n";

        //if object is detected and in the trigger range
        if (dist_to_center <= TRIGGER_DISTANCE + TOLERANCE && dist_to_center > 0) {
            
            //trigger signal can only be sent if object_presence is false
            if (!object_present) {
                std::cout << "Trigger signal sent\n";
                sendTriggerSignal();
                object_present = true;
            }
        } else {
            //if object is not in range, set object_present to false
				object_present = false;
                std::cout << "Object not in range\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }