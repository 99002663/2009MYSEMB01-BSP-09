>Ojective of TFTP is to promote sharing of files, program or data from Host PC to target using the protocol.

*ON Target Side:

Setup server ip adderess in target side "vi /etc/resolv.conf"
  nameserver 192.168.110.10
Setup Gateway route for the transfer of files. route add defaulrt gw 192.168.7.1 usb0

*ON HOST Side
Become a root user "sudo -i"
Enableing the ip forwarding "echo 1>/proc/sys/net/ipv4/ip_forward"
  "sudo iptables --append FORWARD --in-interface enx9059af5b4539 -j ACCEPT"
  " sudo iptables --table nat --append POSTROUTING --out-interface wlx00177c9ba2a8 -j MASQUERADE"
To check the connection is established or not ping on board side "ping 192.168.110.10"

tftp -g 192.168.110.10 -r <filename> 
