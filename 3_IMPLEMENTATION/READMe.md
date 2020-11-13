    >This is a short description of our main project.
    >Our main aim is to create a dummy network driver and to transfer the file using TFTP protocol.
    >We have used Linux as the Interfacing OS as it is one of the safest and secure networking OS and the changes can be done in an easier way.
    >We have created a structure pointer of type net_device which is mainly used to create the network driver.
    >For the different file operations of the dummy driver we have created different functions and linked it with the regular file operations structure named    net_device_ops(defined in netdevice.h)
    >Platform devices are not self discoverable so the kernel needs to provide a way such that the hardware that is actually present can detect it.So we have used a riv structure pointer which detects the platform device.
    >Probe function is used at the time of device boot or when the device is getting connected. A platform device invokes the probe function when a platform device is registered and the device name matches the name specified on the device driver.

    >TFTP is a UDP based application layer protocol.We have used this protocol to send or receive files from the Host PC to the target board.For our implementation, we have used the BeagleBone Black Board as the target board and the Linux based PC as the host.
