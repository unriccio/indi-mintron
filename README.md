# indi-mintron
INDI drivers for Mintron/StellaCam-like cameras remote control

Please see
http://www.dd1us.de/Downloads/Mintron%20Remote%20Controlled%201_2.pdf for
instructions on how to build the RS232 control interface and mimic button
presses remotely.

Keep in mind that the serial interface provides no feedback (only the TX-pin
is connected) and everything is done "dead-reckoning": if you enter in a
sub-menu by hand and then you send commands to the serial port the result
*will be unpredictable*.

## TODO:
- add an internal state machine mirroring the menu structure
  - needs some way to sync/reset the state
  - is it really useful?
