# pic-thermometer
An open source thermometer built with a PIC16F1705 and an MCP9701 thermistor IC. The project contains a compact integer-only version (1KB with printf and libraries included, likely much less without) and a slower and larger but simpler floating point version (almost 6KB). 

By just building the circuit according to the schematic and connecting 5V and ground, the PIC will measure the average ambient temperature averaged over 10 seconds. By changing the samples constant, a longer averaging time can be obtained. 

The floating point version is more accurate, albeit slower. However, it is not as slow as one would imagine it would be given the amount of floating point multiplication and division being used. It is also more accurate than the integer version due to less rounding overall. Surprisingly, even the floating point version is quite fast, and time from acquisition to display is approximately 20mS.

With calibration, using the 10 bit ADC and the excellent FVR peripheral, an accuracy of 0.1C is obtainable. If limited range is acceptable, an accuracy of much less than that can be obtained by lowering the gain on the FVR amplifier. I did not attempt any calibration routines as I have none of the equipment necessary. 

The code is well commented and the circuit is simple. The whole circuit should be easy to build and operate, with a blog post documenting the process coming soon. 
