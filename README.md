# SimpleAudioLights
This little circuit ran my digital lights at the BlueDot Festival 2019.

The sketch was uploaded to a lillipad Arduino which was connected to a MAX 9814 Audio AGC board
This provided audio directly to Pin A0 and through an RC filter at about 2Khz to Pin A1

Selection between which input to use was by a little switch that connected PIN D2 to ground.
D2 is set to PULL_UP so that default is with Anti-Aliasing.

There are 4 channels output on 

#define TREBPIN 8
#define REVERSEPIN 9
#define OUTPUTPIN 10
#define BASEPIN 11


The REVERSEPIN is lit when everything else if off.

The other pins show channels for BASE,MID & TREBLE ranges based on some simple DSP.

Have fun playing with these to change the response of the filters

#define ALPHA 8 //This is the filter used in the Low Pass Filter

#define SIGALPHAAGC 64   //Used in the AGC Calculation

#define SIGRATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define BASERATCHETGAP 10 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...
#define TREBRATCHETGAP 20 //This is the peak detect reset, when it peaks it moves the average up towards the peak, from which it then decays...

#define SIGLATCHLENGTH 1 //Once triggered this is the on dwell time
#define HPLATCHLENGTH 2 //Once triggered this is the on dwell time
#define LPLATCHLENGTH 10 //Once triggered this is the on dwell time


Driving the lights was made possible by using ELWIRE USB Drivers at 5V (Driven by 4.5V from 3x AA Cells) and logic level
N-CHANNEL MosFET in the ground return of those units, the gates driven from output pins.

I found that a capacitor and inductor filter on the VCC line for the Arduino stopped transient low-voltages from crashing
the Arduino so I could run the whole unit from the same power supply.

