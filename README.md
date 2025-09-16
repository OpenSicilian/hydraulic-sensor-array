Project Aim: Multi-Node RS485 System. Sensor module includes: flow sensor, pressure sensor, mag sensor, temp sensor

2 Wire RS485 nodes cannot perform loopback alone, additional node or full duplex is required
Pico clones aren't fantastic: Issue with sniffing serial after Arduino IDE flashing....
DT-5019 USB2.0 to RS485 --> silicon labs cp210x UART bridge OR!!!!!!! FTDI chipset may be used
pico-2ch-rs485 --> full or half duplex RE DE pulled low / high by TX RX (no need to set!?)
Serial not as above: Serial = usb, serial1 = 0, serial2 --> GP4,5
Readme --> https://electronics.stackexchange.com/questions/619023/how-does-this-rs485-transceiver-work
