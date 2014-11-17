README
------

Use libpcap and SDL to visualize IP traffic between multiple endpoints

Color code:
On packet-balls:
    - Red for UDP
    - Green for TCP
    - White/blue for any other IP protocol
On hosts:
    - I still don't know.


On the gui, you can also use the mouse and the keyboard.

Keyboard keys:
    - ! to open the menu
    - , to start displaying the capture (Load button).
    - p to pause and play the capture
    - <ESC> to disconnnect?

You need to install these on debian-based distros
-------------------------------------------------
- apt-get install libcrypto++-dev libsdl-image1.2-dev libpcap-dev libsdl1.2-dev libfreetype6-dev g++ make

Installation
------------
Type make install
If you have any dependency error, just install them.

You can modifiy the oip.conf file to suit your needs but it is optional.

Authors
-------
Rian Shelley (Utah State Univeristy)
Additional features added by Sebastian Garcia (eldraco@gmail.com) and Vojtech Uhlir (wojtyla@agents.felk.cvut.cz) (Czech Technical University)

