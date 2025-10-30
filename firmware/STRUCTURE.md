## structure 

```
firmware/
├── platformio.ini          # PlatformIO configuration
├── include/                # Global configuration headers
│   └── bibi_config.h       # System-wide configuration and pin definitions
├── src/                    # Main application code
│   └── main.c             
└── lib/                    # Modular subsystems and drivers
    ├── display_driver/     # Display hardware drivers
    └── face_subsystem/     # Face animation logic
```

## design

### 1. **Separation of Concerns**
- **`src/`**: High-level application logic and system initialization
- **`lib/`**: Reusable, modular components organized by function
- **`include/`**: Global config shared across modules

### 2. **Driver vs. Subsystem**
- **Drivers** (`lib/XXX_driver/`): Low-level hardware abstraction
  - Direct hardware interfacing
  - No business logic
  - Example: Display driver, motor driver
  
- **Subsystems** (`lib/XXX_subsystem/`): High-level functional modules
  - Business logic and feature implementation
  - Uses drivers to interact with hardware
  - Example: Face subsystem, navigation subsystem


> Any directory in `lib/` is automatically added to the include path

## adding new components

### driver example

For example, adding motor drivers:

```
lib/
└── motor_driver/
    ├── motor_driver.c
    ├── motor_driver.h
    └── README.md
```

### subsystem example

For example, adding navigation:

```
lib/
└── navigation_subsystem/
    ├── navigation.c
    ├── navigation.h
    ├── pid_controller.c
    ├── path_planning.c
    └── README.md
```

### sensor example

```
lib/
└── sensor_driver/
    ├── ultrasonic.c
    ├── ultrasonic.h
    ├── imu.c
    ├── imu.h
    └── README.md
```
