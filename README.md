# cm4bmp
Some practices on ARM Cortex-M4 processor to learn M4 Architecture.
- Device: [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)

## Build
`cm4bmp` dependents on some third-party packages to build code and burn into processor like [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) and [stlink](https://github.com/stlink-org/stlink). Therefore, before using the project, we need to install these packages.

Install [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain):
```shell
$ sudo apt-get update
$ sudo apt-get -y install gcc-arm-none-eabi
```

Install [stlink](https://github.com/stlink-org/stlink):
```shell
$ sudo apt-get install git cmake libusb-1.0-0-dev
$ git clone https://github.com/stlink-org/stlink && cd stlink
$ cmake .
$ make
$ cd bin && sudo cp st-* /usr/local/bin
$ cd ../lib && sudo cp *.so* /lib32
$ cd ../config/udev/rules.d/ && sudo cp 49-stlinkv* /etc/udev/rules.d/
```

Install [OpenOCD](https://openocd.org/):
```shell
$ sudo apt install openocd
```

Assign the file we want to run. For example:
```
BIN = $(shell pwd)/test/hello-world/main
```

Compile the file:
```shell
$ make
```

Burn the code into processor:
```shell
$ make upload
```

## Debug
In `cm4bmp`, it uses [OpenOCD](https://openocd.org/) to open the debug mode of processor. We can use the following command:
```shell
$ make debug
```

After typing the command, the next step is opening gdb to connect to rhe openocd.
```shell
$ arm-none-eabi-gdb
```

Connect to the openocd:
```shell
$ target remote localhost:3333
```

## Tests
In `cm4bmp`, there are different example code on Cortex-M4 processor. It can be found in repository `test`.
- Done
  - hello-world
  - bit band operation
  - inline assembly
- Not done
  - clock configuration
  - exception configuration
  - interrupt configuration
  - task scheduler
  - stack pointer configuration

## Reference
- [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)
- [OpenOCD](https://openocd.org/)
- [stlink](https://github.com/stlink-org/stlink)
- [Cortex-M4 practices](https://hackmd.io/@Risheng/cm4bmp)
- [Clock configuration](https://hackmd.io/@Risheng/cm4-clock)
