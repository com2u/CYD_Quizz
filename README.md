# ESP32 CYD (Cheap Yellow Display) Project

This project utilizes an ESP32 with a 2.8" TFT touch display (commonly known as "Cheap Yellow Display" or CYD) to create a versatile interactive device. It includes features such as a countdown timer, sound playback, Wi-Fi connectivity, ping monitoring, and image display capabilities.

https://github.com/com2u/CYD_Quizz/blob/main/data/CYD.png

## Hardware Requirements

- ESP32 with 2.8" TFT touch display (ESP32-2432S028)
- XPT2046 Touchscreen controller
- I2S DAC for audio output

## Software Dependencies

- Arduino IDE with ESP32 board support
- TFT_eSPI library
- XPT2046_Touchscreen library
- TJpg_Decoder library
- Audio library (ESP32-audioI2S)
- WiFi library

## Features

1. **Interactive Menu System**: Navigate through various functions using the touch screen.
2. **Time Display**: Shows current time (requires internet connection for synchronization).
3. **Countdown Timer**: Set and display a countdown.
4. **Sound Board**: Play various audio samples.
5. **Network Ping**: Monitor network connectivity.
6. **Image Display**: Show JPEG images from URLs.
7. **Count Up Timer**: A simple count-up timer.
8. **On-screen Keyboard**: Input text using a touch keyboard.
9. **Audio Volume Control**: Adjust the volume of audio playback.
10. **RGB LED Control**: Adjust the color of an RGB LED (if connected).

## Setup

1. Install the required libraries in your Arduino IDE.
2. Set up your Wi-Fi credentials in the `Credentials.h` file (you need to create this).
3. Adjust pin configurations in `CYD_JPEG.h` and `CYD_AudioESP32.h` if your hardware differs.
4. Set the partition scheme to "Huge APP (3MB No OTA/1MB SPIFFS)" in the Arduino IDE.
5. Compile and upload the sketch to your ESP32.

## Usage

After powering on, the device will attempt to connect to Wi-Fi. Once connected, you can navigate through the menu options using the touch screen. Each menu item corresponds to a different function of the device.

## Files

- `CYD_Quizz.ino`: Main Arduino sketch file.
- `CYD_JPEG.h`: Handles JPEG decoding and display.
- `CYD_TFT.h`: Manages the TFT display and touch interactions.
- `CYD_WIFI.h`: Handles Wi-Fi connectivity.
- `CYD_AudioESP32.h`: Manages audio playback.

## Contributing

Contributions to improve the project are welcome. Please fork the repository and submit a pull request with your changes.

## License

[Insert your chosen license here]

## Acknowledgments

- Thanks to the creators and maintainers of the libraries used in this project.
- Inspired by various ESP32 projects in the maker community.

Note: This project is designed for educational and experimental purposes. Use at your own risk.
