# Robot Controller

This project is designed to control a robot using several classes implemented in C++. The project is developed for Arduino and is intended to be compiled and run on an Arduino-compatible board.

## Project Structure

```
robot-controller
├── robot-controller.ino     # Main Arduino sketch
├── include
│   ├── InverseMotorModel.h  # Declaration of the InverseMotorModel class
│   ├── Motor.h              # Declaration of the Motor class
│   ├── MotorController.h    # Declaration of the MotorController class
│   ├── Robot.h              # Declaration of the Robot class
│   ├── Trajectory.h         # Declaration of the Trajectory class
│   └── Utils.h              # Declaration of utility functions
├── lib
│   └── Makeblock-Libraries-master.zip  # External libraries
├── src
│   ├── InverseMotorModel.cpp # Implementation of the InverseMotorModel class
│   ├── Motor.cpp             # Implementation of the Motor class
│   ├── MotorController.cpp   # Implementation of the MotorController class
│   ├── Robot.cpp             # Implementation of the Robot class
│   ├── Trajectory.cpp        # Implementation of the Trajectory class
│   └── Utils.cpp             # Implementation of utility functions
├── tests
│   ├── motor.ino             # Test sketch for Motor class
│   ├── motorController.ino   # Test sketch for MotorController class
│   ├── robot_withAPI.ino     # Test sketch for Robot class with API
│   ├── robot.ino             # Test sketch for Robot class
│   ├── trajectory_withAPI.ino # Test sketch for Trajectory class with API
│   └── trajectory.ino        # Test sketch for Trajectory class
└── README.md                 # Project documentation
```

## Setup Instructions

1. **Clone the repository:**
   ```sh
   git clone https://github.com/yourusername/robot-controller.git
   cd robot-controller
   ```

2. **Install the Arduino IDE:**
   Download and install the Arduino IDE from the [official website](https://www.arduino.cc/en/software).

3. **Import the library:**
   - Open the Arduino IDE.
   - Go to `Sketch` > `Include Library` > `Add .ZIP Library...`.
   - Navigate to the `lib` directory and select `Makeblock-Libraries-master.zip`.

4. **Open the main sketch:**
   - Go to `File` > `Open...`.
   - Navigate to the `robot-controller` directory and open `robot-controller.ino`.

5. **Upload the sketch to your Arduino board:**
   - Connect your Arduino board to your computer.
   - Select the appropriate board and port from the `Tools` menu.
   - Click the `Upload` button in the Arduino IDE.

## Usage

- The `Motor` class provides methods to initialize motors, set their speed, and stop them.
- The `MotorController` class provides methods to convert a desired motor speed to a tension using a PI controller.
- The `InverseMotorModel` class provides methods to convert a desired x,y position to a desired motor speed for the left and right motors.
- The `Robot` class provides methods to control the robot's movement and update its state.
- The `Trajectory` class provides methods to generate a polynomial trajectory using an initial and a final state.
- Utility functions in the `Utils` class assist with value conversions.