README
------

Use libpcap and SDL to visualize IP traffic between multiple endpoints

Packages
--------
 * `deb http://mirror.usu.edu/usu-security/ [codename]-security-unstable main`
     * Debian: sid, bullseye
     * Ubuntu: impish, focal, bionic
 * install `usu-archive-keyring` package and apt-get update
     * keyring file available at `https://raw.githubusercontent.com/USU-Security/oip/master/usu_archive_keyring.gpg`

Colors
------
On packet-balls:
 - Red for UDP
 - Green for TCP
 - White/blue for any other IP protocol
On hosts:
 - defined by oip.conf (ABGR, with A ignored)


Running
-------
Steps to run the server/client for a live capture
 - create /etc/oip.conf and at a minimum set a secret
 - start the server: `oipd <interface>`
 - start the client: `oip -f <pcap_filter> -s <server>`

The client also supports reading from a pcap file
 - `oip -c <pcap_file>`, can use `-e <number>` to change speed (1000 is default, number of ms per 1 second in the pcap)

Controls
--------
On the gui, you can use the mouse and the keyboard.

Mouse controls:
 - left click on host to reposition
 - right click on host to bring up context menu
 - add actions to context menu in oip.conf on client

Keyboard keys:
 - ! to open the menu
 - , to start displaying the capture (Load button).
 - p to pause and play the capture
 - <ESC> to disconnnect?

Dependencies
------------
 - `apt-get install libcrypto++-dev libsdl-image1.2-dev libpcap-dev libsdl1.2-dev libfreetype6-dev g++ make libfontconfig1-dev`

Installation
------------
Type 
```
./autogen.sh
./configure
make
make install
```

If you have any dependency error, just install them.

You can modifiy the oip.conf file to suit your needs but it is optional.

Authors
-------
Rian Shelley (Utah State Univeristy)
Maintained by Eldon Koyle (Utah State University)
Additional features added by Sebastian Garcia (eldraco@gmail.com) and Vojtech Uhlir (wojtyla@agents.felk.cvut.cz) (Czech Technical University)

