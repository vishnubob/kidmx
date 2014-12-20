=kidmx=

kidmx is an Ethernet to DMX bridge.  Currently, it only supports kinet and the Raspberry Pi.

==Features==

* kinet support
* Multiple Universe support (limited by your GPIO)
* FAST!

==Installation==

* You need to install [pigpio][http://abyz.co.uk/rpi/pigpio/] for this to compile.
* You have to run this as root.

==Running==

Let's say you have two DMX universes, one connected to GPIO20 and the other connected to GPIO21.  You will need to configure your ethernet interfaces with two aliases.  For example, 192.168.1.100 and 192.168.1.200.  As root, here is how you would configure your system.

<pre>
# ifconfig eth0:1 192.168.1.100
# ifconfig eth0:2 192.168.1.200
# kidmx 20 192.168.1.100 21 192.168.1.200
</pre>
