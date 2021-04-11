# ft_ping
42 project that requires reimplementation in c the 'ping' utility, used for testing and network diagnostics, using raw sockets to send ICMP echo requests and waits for ICMP echo replies and print information about received replies or suitable error messages when not.

> **note** : the program requires root privileges, they are required to open raw sockets.

> **note** : the program is designed to run only on Linux machines;

### system requirements

make, gcc


## compilation
for compilation, just go to project directory and run :
```bash
make
```

## usage example

```bash
$ sudo ./ft_ping google.com                          
PING google.com (172.217.19.46), 56(84) bytes of data
64 bytes from mrs08s03-in-f14.1e100.net (172.217.19.46): icmp_seq=1 ttl=111 time=27.4 ms
64 bytes from mrs08s03-in-f14.1e100.net (172.217.19.46): icmp_seq=2 ttl=111 time=26.7 ms
64 bytes from mrs08s03-in-f14.1e100.net (172.217.19.46): icmp_seq=3 ttl=111 time=26.8 ms
^C
--- google.com ping statistics ---
3 packets transmitted, 3 received, 0.000000% packet loss, time 2830ms
rtt min/avg/max/ = 26.688/26.955/27.411 ms
```
> **note** : press CTRL-C to execute the program and vew statistcis

for more otions :
```bash
./ft_ping -h
```