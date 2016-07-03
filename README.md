# indi-mintron
INDI drivers for Mintron/StellaCam-like cameras remote control
(just the management/control part, the video stream is analog).

There are two different drivers, one for Mintron serial protocol and one for
the "fake" remote control interface. Perhaps one day I will merge them.

## Mintron serial protocol
Please see
http://www.mintron.com/HTM/New_PRODUCTS/ALPHA/download/MTV-63V5HMU.pdf

Compile with: `g++ $(pkg-config --libs --cflags libindi) -lindidriver mintron_serial.cpp -o indi_mintron_serial

### TODO:
- is there some way to read all settings with a single command?
- (related to the last one) someone physically near the camera could alter
  settings pressing the buttons without the driver knowing,
  - does the camera send some kind of irq?
  - else we need a parameter signaling to the driver it could *assume*
    exclusive access
  - or else the driver should reread all parameters every now and then (if the
    readout is fast this is the safest thing to do anyway)
- is possible to set the overlay title through the serial protocol (it's not
  ASCII, there is a text/patterns map to follow):
  - how can this be mapped into INDI parameters? (char01, char02, ... char12?)
  - could be useful having a text input field? (just for uppercase/lowercase
    latin characters + numbers + puntuaction, everything else must be input in
    some other way)


## "fake" remote control
Please see
http://www.dd1us.de/Downloads/Mintron%20Remote%20Controlled%201_2.pdf for
instructions on how to build the RS232 control interface and mimic button
presses remotely.

See also:
- http://www.dd1us.de/astro%20downloads.html (^F mintron).

Keep in mind that the serial interface provides no feedback (only the TX-pin
is connected) and everything is done "dead-reckoning": if you enter in a
sub-menu by hand and then you send commands to the serial port the result
*will be unpredictable*.

Compile with: `g++ $(pkg-config --libs --cflags libindi) -lindidriver mintron_fake.cpp -o indi_mintron_fake

### TODO:
- add an internal state machine mirroring the menu structure
  - needs some way to sync/reset the state
  - is it really useful? (could be useful when we merge the two drivers)
