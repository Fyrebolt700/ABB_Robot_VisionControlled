# RobotVisionTrigger

This project integrates an Intel RealSense depth camera with ABB Robot Studio to create a vision-triggered robotic movement system. The C++ program continuously monitors the distance of objects in front of the camera and sends a trigger signal to the robot when an object enters a defined range. The RAPID program running on the robot listens for this signal and executes a pre-defined motion sequence.

## Features

- Real-time object detection using Intel RealSense depth camera
- TCP socket communication between PC and ABB Robot Studio
- RAPID program automatically executes robot motion upon receiving a trigger
- Adjustable distance thresholds for trigger activation

## Technologies Used

- Intel RealSense SDK
- C++
- ABB Robot Studio / RAPID
- TCP Socket Programming

## Setup

### 1. ABB Robot Studio (RAPID)

1. Load the RAPID program (`Module1`) onto your ABB robot or simulator.
2. Ensure the robot’s IP and port match the C++ program's configuration.
3. Start the RAPID program and wait for a client connection.

### 2. C++ Program

1. Install Intel RealSense SDK and configure your camera.
2. Compile the C++ program.
3. Ensure the IP and port match the RAPID program.
4. Run the program — it will monitor the depth camera and send a trigger signal when an object enters the defined range.

## Usage

1. Position an object in front of the RealSense camera.
2. The C++ program will detect the object’s distance.
3. When the object is within the trigger range, a TCP signal is sent to the robot.
4. The robot executes the pre-defined movement sequence in RAPID.

## Notes

- Ensure that the TCP port is open and not blocked by firewall settings.
- Adjust `TRIGGER_DISTANCE` and `TOLERANCE` in the C++ code to suit your setup.
- The RAPID program waits for the "GO" signal before executing any movements.

## License

This project is open-source. Feel free to use and modify it for educational and research purposes.
