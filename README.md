Arduino blackbox port scanner

This is a little project to create a arduino port scanner.
Once plugged into the network the arduino will request an IP from
the local DHCP server then start pinging local IPs until an active
system is found, then ports will be scanned and open ports will be
written to local SD card.

TO DO:
- configure to use wifi shield in replacement of ethernet shield.
