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

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[CC0-1.0](https://creativecommons.org/publicdomain/zero/1.0/)
