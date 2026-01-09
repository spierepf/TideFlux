# TideFlux

TideFlux is an ESP32 application for reading sensors and sending OSC updates to an instance of Tidal Cycles.

## Installation

You will need to create a `secret.ini` file in the root folder of this project with the following contents (replacing `my_ssid` and `my_psk` with the name and password for your wifi network:

```ini
[secret]
WIFI_SSID = \"my_ssid\"
WIFI_PSK = \"my_psk\"
```

You can then use PlatformIO to build the project and install it onto your ESP32.

## Hardware

This is a basic circuit for wiring a single potentiometer (10Kohm recommended) to an ESP32.

<img width="587" height="787" alt="hardware" src="https://github.com/user-attachments/assets/5d2243a5-cd10-463e-8bbc-ee79ab491509" />

Ensure that the ESP32 is not connected to your computer while assembling or altering the circuit. Also, be sure to use the 3.3V rail (and not the 5V) to avoid overvolting the ESP32's gpios (which can only handle 3.3V).

This circuit will provide a voltage to GPIO32 that varies according to the position of the potentiometer. This voltage will in turn be translated into a floating point value between 0.0 and 1.0 (inclusive) and that value will be broadcast via UDP on port 6060 using the OSC protocol.

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[CC0-1.0](https://creativecommons.org/publicdomain/zero/1.0/)
