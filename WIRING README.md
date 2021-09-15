# Fanatec-Shifter-USB-Adapter
Arduino Micro Adapter for USB with Handbrake Support

I do not have asketch for this but the inputs are easily configuarable within the Arduino Sketch
Most of my information for this came from this guy, https://hackaday.io/project/171155-fanatec-clubsport-shifter-sq-v15-usb-adapter-diy
However the Micro could not use some of his logic(I believe, I never spent much time trying to fiure out why his sketch wouldnt function on mine)


The Biggest piece of information is the outputs for the RJ12

1-GND
2-Sequential Switch, Low=H pattern High=Sequential
3-internally shorted to pin 2
4-X axis (H-pattern); lever push (sequential)
5-Y axis (H-pattern); lever pull (sequential)
6-VCC

For this Sketch on a Micro The wiring is as follows:
A0- Will be wired to the Mode switch
A1- X Axis
A2- Y axis
A3- Optional Potentiometer for Handbrake.

Obviously gorund and VCC will need to coincide with the Micro as well.
