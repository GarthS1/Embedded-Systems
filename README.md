# Embedded System Interfacing
The following projects are projects done in Embedded System Interfacing Course of University of Calgary. We used PIC24F16KA101 to test and run the program. For more information about the hardware, please refer to the following website: [PIC24F16KA102 Family Data Sheet](https://ww1.microchip.com/downloads/en/DeviceDoc/39927c.pdf)

# Author
* [Garth Slaney](https://github.com/GarthS1)
* [Viet Long Ta](https://github.com/longtv2222)
* [Weitao Wu](https://github.com/VictorWu00)

# Project Descriptions:

Driver Project 2: When push button one pressed blink light at 1-sec intervals, push-button two pressed blink light at 2-sec intervals, push-button three pressed blink light at 3-sec intervals, more then two pushed light stays on and if nothing is pushed light stays off. 

Driver Project 3: Same as driver project 2 but instead of using polling(calling from main while loop) use interrupts instead. 

Driver Project 4: Similar to driver project 3 but also displays on the screen which buttons are being pressed. 

Driver Project 5: Perform an analog to digital conversion of the analog voltage from the potentiometer from pin 8 and display the value in hex. Also, generate a bar graph where the width corresponds to the digital output value.

Application Project 1: Make a countdown timer with the following features; button one increases the minutes, button two increases the seconds, a short button three press starts the countdown and a long button three press resets the timer to zero. 

Application Project 2: Use the analog to digital conversion to measure the voltage from pin 8 if button one is pressed and the resistance at pin 13 if button two is pressed. 

Midterm Project: Add onto Application Project 2 the ability when pressing button three to measure the pulses at pin 15 from pin 18. 
