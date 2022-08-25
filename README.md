# micro_ros_stm32_uart_template
Talker-listener demo with Micro-ROS, STM32F4 and mbed framework over UART transport.

**[STM32 (Core2)](https://husarion.com/manuals/core2/) + [micro-ROS](https://micro.ros.org/) + [mBed](https://os.mbed.com/) + [PlatformIO](https://platformio.org/)**

## Flashing STM32

Click **[Use this template](https://github.com/husarion/micro_ros_stm32_uart_template/generate)** button, and clone your own copy of this repo.

Open the project with a [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO extension](https://platformio.org/install/ide?install=vscode).

```bash
# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Clean build files
$ pio run --target clean

```

### ST-Link

uncomment:

```
upload_protocol = stlink
```

and click **PlatformIO: Upload** button.

### Serial

uncomment:

```
upload_protocol = serial
```

Set `BOOT0` pin to HIGH and click `RESET` button, to enter the programming mode.

Click **PlatformIO: Upload** button.

Set `BOOT0` pin to LOW and click `RESET` to start newly flashed firmware.

## Running a demo
> **Warning**
> This example doesn't support agent reconnection! Firstly run micro-ros-agent then turn on or restart micro controller.
> [micro_ros_arduino reconnection example](https://github.com/micro-ROS/micro_ros_arduino/blob/humble/examples/micro-ros_reconnection_example/micro-ros_reconnection_example.ino)

```bash
cd demo
docker compose up
```