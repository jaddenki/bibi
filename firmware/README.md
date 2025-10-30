# bibi firmware! 

Use the VSCode PlatformIO extension & press `Upload and Monitor`

## structure

```
firmware/
├── platformio.ini          # Build configuration
├── include/                # Global headers
│   └── bibi_config.h       # Pin definitions & config
├── src/                    # Main application
│   └── main.c              # Entry point
└── lib/                    # Modular components (drivers & subsystems)
```

See [STRUCTURE.md](STRUCTURE.md) for organization info.

## config

All pin configurations are centralized in `include/bibi_config.h`.
