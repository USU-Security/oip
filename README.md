OIP
---

OrganicIP (OIP) visualizer uses libpcap and Simple DirectMedia Layer (SDL) to visualize IP traffic between endpoints.

Supported Distributions
-----------------------

Any linux distribution with the necessary [dependencies](#dependencies) is supported with [installation instructions](#installation-from-source). Pre-built packages are available for the following operating systems are supported: 

- Debian
   - sid
   - bullseye
- Ubuntu 
   - impish
   - focal
   - bionic

Distribution
------------

While mirrors are available at following location for older distributions they longer actively maintained:

http://mirror.usu.edu/usu-security/dists/

The repository keyring is located at the following location but has expired:

https://raw.githubusercontent.com/USU-Security/oip/master/usu_archive_keyring.gpg

We encourage you to compile oip for the latest operating system distributions e.g., Ubuntu Noble. 

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
 - , to start displaying the capture (Load button)
 - p to pause and play the capture
 - <ESC> to disconnect

Installation from Repository
----------------------------

For older Ubuntu distributions, to install OIP from existing repositories execute the following commands:

```
curl -L https://raw.githubusercontent.com/USU-Security/oip/master/usu_archive_keyring.gpg | sudo apt-key add -
sudo add-apt-repository 'deb [arch=amd64] http://mirror.usu.edu/usu-security/ focal-security-unstable main'
sudo apt-get install oip
cp /usr/share/doc/oip/oip.conf-example /etc/oip.conf
```

Configuring the firewall and placing the relevant interface into promiscuous mode is encouraged.

Dependencies
------------

The following are the dependent packages needed for OIP to compile: 

 ```
 apt-get install libcrypto++-dev libsdl-image1.2-dev libpcap-dev libsdl1.2-dev libfreetype6-dev g++ make libfontconfig1-dev autoconf
 ```

Installation from Source
------------------------

To compile OIP from source execute the following commands:
```
git clone https://github.com/USU-Security/oip.git
cd oip
./autogen.sh
./configure
make
make install
```

If you have any dependency error, just install them.

Authors
-------

- Rian Shelley (Utah State University)
- Maintained by Eldon Koyle (Utah State University)
- Additional features added by Sebastian Garcia (eldraco@gmail.com) and Vojtech Uhlir (wojtyla@agents.felk.cvut.cz) (Czech Technical University)