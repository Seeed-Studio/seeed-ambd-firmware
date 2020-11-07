# Seeed RTL872X RPC firmware  [![Build Status](https://travis-ci.com/Seeed-Studio/seeed-ambd-firmware.svg?branch=master)](https://travis-ci.com/Seeed-Studio/seeed-ambd-firmware)

## Introduction

This RTL87XX [RPC](https://github.com/EmbeddedRPC/eRPC) firmware export a RPC server interface through hardware SPI/UART port to MCU.  

## How to compile 
### Tools 
The arduino-cli tool is used to build and upload the RTL8720DN firmware to the Seeed Wio terminal board. Use following link for download and installation procedure:
* [Arduino CLI](https://arduino.github.io/arduino-cli/installation/).
### ArduinoCore
Before compiling the firmware, you need to install the Arduino core of rtl872x [ArduinoCore-ambd](https://github.com/Seeed-Studio/ArduinoCore-ambd/)
- board index
```
https://files.seeedstudio.com/arduino/package_realtek.com_amebad_index.json
```

### build
```sh
chmod +x build.sh
./build.sh --build
```

### flash

```sh
chmod +x build.sh
./build.sh --flash /dev/tty***
````

-----
This software RPC server section is written by Seeed Studio
and is licensed under The MIT License. Check License.txt for more information.

Contributing to this software is warmly welcomed. You can do this basically by
forking, committing modifications and then pulling requests (follow the links above
for operating guide). Adding change log and your contact into file header is encouraged.
Thanks for your contribution.

