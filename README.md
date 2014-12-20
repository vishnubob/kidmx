# kidmx

`kidmx` is an fast multi-universe ethernet to DMX bridge.  Currently, it only supports kinet and the Raspberry Pi.

## Features

* kinet support
* Multi-universe support
* Fast
* Accurate

## Installation

* You need to install [pigpio](http://abyz.co.uk/rpi/pigpio/) for this to compile.
* You have to run `kidmx` as root.

## Running

Let's say you have two DMX universes, one connected to `GPIO20` and the other connected to `GPIO21`.  You will need to configure your ethernet interfaces with two aliases.  For example, `192.168.1.100` and `192.168.1.200`.  As root, this is how you would configure your system:

```
# ifconfig eth0:1 192.168.1.100
# ifconfig eth0:2 192.168.1.200
# ./kidmx 20 192.168.1.100 21 192.168.1.200
```

In this example, `GPIO20` is wired to `192.168.1.100` and `GPIO21` is wires to `192.168.1.200`.
