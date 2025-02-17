# Robot Controller

This project is designed to control a robot using a MotorController class implemented in C++. The project is developed for Linux and is intended to be compiled and run on a Raspberry Pi.

## Project Structure

```
robot-controller
├── src
│   ├── main.cpp             # Entry point of the application
│   ├── MotorController.cpp   # Implementation of the MotorController class
│   ├── MotorController.h     # Declaration of the MotorController class
│   └── utils
│       ├── Utils.cpp        # Implementation of utility functions
│       └── Utils.h          # Declaration of utility functions
├── include
│   ├── MotorController.h     # External access to MotorController class
│   └── Utils.h               # External access to utility functions
├── CMakeLists.txt           # CMake configuration file
└── README.md                # Project documentation
```

## Setup Instructions

1. **Clone the repository:**
   ```
   git clone https://github.com/yourusername/robot-controller.git
   cd robot-controller
   ```

2. **Install dependencies:**
   Ensure you have CMake and a compatible C++ compiler installed on your Raspberry Pi.

3. **Build the project:**
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the application:**
   ```
   ./robot-controller
   ```

## Usage

- The `MotorController` class provides methods to initialize motors, set their speed, and stop them.
- Utility functions in the `Utils` class assist with sensor data handling and value conversions.

## Contributing

Feel free to submit issues or pull requests for improvements or bug fixes.