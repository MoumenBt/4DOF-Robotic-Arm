# Robotic Arm Project

This repository contains the code and documentation for a 4-DOF robotic arm that can be controlled using both a joystick and a custom mobile application developed with MIT App Inventor.

## Project Overview
The robotic arm is designed to be flexible and user-friendly, allowing control through:
- A **joystick** for precise manual operation.
- A **mobile application** created using **MIT App Inventor** for wireless control.

## Code Files
- **Robotic_arm_master_Code.ino**: Handles the main control logic and communication with the slave unit.
- **Robotic_arm_slave_Code.ino**: Manages the movement of the robotic arm based on the received commands.

## Features
- **Dual Control Mode**: The robotic arm can be operated using either a physical joystick or the mobile application.
- **Wireless Communication**: The system allows wireless control through the MIT App Inventor-based app.
- **Multi-DOF Movement**: Supports multiple degrees of freedom for precise arm control.
- **Mouvments record**: The robotic arm can record mouvments and play it back by pushing a button in the app or in the controller.

## Setup & Usage
1. **Upload the Codes**: Flash the respective Arduino boards with the master and slave codes.
2. **Connect the Components**: Ensure proper wiring between the controllers, servos, and communication modules.
3. **Use the Joystick**: Connect and operate the robotic arm manually.
4. **Use the Mobile App**:
   - Install the MIT App Inventor-generated app on your Android device.
   - Pair it with the robotic arm controller.
   - Control the arm wirelessly via the app interface.

## Screenshot of Mobile Application
A screenshot is attached in the repository

## Screenshot of Mobile Application
A scheme that shows connections is attached in the repository

## Future Improvements
- Adding feedback sensors for precise movement tracking.
- Implementing computer vision for automated control.
- Enhancing the mobile app with more features.

For any issues or contributions, feel free to open a pull request or issue in this repository.

