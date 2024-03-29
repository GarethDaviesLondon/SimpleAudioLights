# SimpleAudioLights

This little circuit ran my digital lights at the BlueDot Festival 2019, Jodrell Bank, England

See it working here: 
https://twitter.com/0cit/status/1155867390607056896


The sketch was uploaded to a lillipad Arduino which was connected to a MAX 9814 Audio AGC board
This provided audio directly to Pin A0 and through an RC low-pass filter on the audio out, set at at about 2Khz and connected to Pin A1. This cleaned up the signal and made the filters behave as expected.

Selection between which input to use was by a little switch that connected PIN D2 to ground.
D2 is set to PULL_UP so that default is with Anti-Aliasing.

There are 4 channels output on digital pins

TREBPIN 8
REVERSEPIN 9
OUTPUTPIN 10
BASEPIN 11


The REVERSEPIN is lit when everything else if off.

The other pins show channels for BASE,MID & TREBLE ranges based on some simple DSP.

Have fun playing with these to change the response of the filters

These are the low-pass filtering average calculations.

define ALPHA 8 //This is the filter used in the Low Pass Filter

define SIGALPHAAGC 64   //Used in the AGC Calculation


There are two forms of AGC in the circuit, one on the analogue side from the MAXIM chip, the other is by moving the peak-detection trigger up. These gap figures help with that process.

define SIGRATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...

define BASERATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...

define TREBRATCHETGAP 20 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...

-The Latching effect removes high frequency flicker and makes it more positive.

define SIGLATCHLENGTH 1 //Once triggered this is the on dwell time

define HPLATCHLENGTH 2 //Once triggered this is the on dwell time

define LPLATCHLENGTH 10 //Once triggered this is the on dwell time


Driving the lights was made possible by using ELWIRE USB Drivers at 5V (Driven by 4.5V from 3x AA Cells) and logic level
N-CHANNEL MosFET in the ground return of those units, the gates of the FETs are driven from output pins directly.

A 10uf capacitor and 33uH inductor filter on the VCC line for the Arduino stopped transient low-voltages from crashing
the Arduino so I could run the whole unit from the same power supply.

