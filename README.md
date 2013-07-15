This is an update and modification of th OIP project to run on a Debian 7.0.

I took the original code from https://it.wiki.usu.edu/OIP and try to run it, but unfortunately it didn't work.
I changed some c++ calls and now it compiles and works fine.

Changes made:
- Minimal fixes to run under Debian
- Enlarge the size of the packets-balls so you can see them better.
- Change the blue balls to white balls, to see it better with dark backgrounds.
- Add the -e argument to speed up the analysis of pcap files. 
    -e 1000 is normal
    -e 100 is 10x faster
    -e 10 is 100x faster
- Add the -c argument to give the pcap file name in the command line.
    - If you give a pcap file name, the analysis starts right away.
- Pause the capture by pressing the letter p.
- Play again the capture by pressing the letter p.


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
    - p to pause and play the capture

 
It was and still is a GPLv3 project, so enjoy and have fun.

You need to install these on debian-based distros
-------------------------------------------------
- apt-get install libcrypto++-dev libsdl-image1.2-dev libpcap-dev libsdl1.2-dev libfreetype6-dev g++ make

Installation
------------
Type make
If you have any dependency error, just install them.

You can modifiy the oip.conf file to suite your needs but it is optional.

Authors
-------
Original code was made by Rian Shelley (Utah State Univeristy).
New version in this git code is made by Sebastian Garcia (eldraco@gmail.com) and Vojtech Uhlir (wojtyla@agents.felk.cvut.cz) (Czech Technical University)

TODO
- Put the color references on the screen.
- Advance and rewind the packets.


Thanks to the usu.edu for the project.
