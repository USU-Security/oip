This is an update and modification of th OIP project to run on a Debian 7.0.

I took the original code from https://it.wiki.usu.edu/OIP and try to run it, but unfortunately it didn't work.
I changed some c++ calls and now it compiles and works fine.

Changes I made:
- Minimal fixes to run under Debian
- Enlarge the size of the packets-balls so you can see them better.
- Change the blue balls to white balls, to see it better with dark backgrounds.
- Add the -e argument to speed up the analysis of pcap files. 
    -e 1000 is normal
    -e 100 is 10x faster
    -e 10 is 100x faster
- Add the -c argument to give the pcap file name in the command line.
    - If you give a pcap file name, the analysis starts right away.


Color code:
On packet-balls:
    - Red for UDP
    - Green for TCP
    - White for ICMP
On hosts:
    - I still don't know.

On the gui, you can also use the mouse and the keyboard.

Keyboard keys:
    - ! to open the menu
    - , to start displaying the capture (Load button).

 
You need to install these on debian-based distros:

apt-get install libcrypto++-dev libsdl2-dev libsdl-image1.2-dev libpcap-dev

It was and still is a GPLv3 project, so enjoy and have fun.

Needed libraries on Debian:
- apt-get install libcrypto++-dev libsdl-image1.2-dev libpcap-dev libsdl1.2-dev libfreetype6-dev g++ make

To compile type:
make

You should modifiy the oip.conf file to suite your needs. It is working fine.

Thanks to the usu.edu for the project.

sebas garcia (eldraco@gmail.com) made the small modifications and the github code.

TODO
- Put the color references on the screen.
