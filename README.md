# EX-DCC-Turntable

This README is a very brief summary, for the full documentation relating to EX-Turntable, please refer to the [DCC-EX Website](https://dcc-ex.com/).

It is developed for use with the RT_EX-Turntable board as available here:

https://rosscoe.com/shop/index.php?main_page=product_info&cPath=7&products_id=24



**AccelStepper.h credit:** This project would not be effective without the excellent work by Mike McCauley on the AccelStepper.h library that enables us to have somewhat prototypical acceleration and deceleration of the turntable. More details on the library can be found on the official [AccelStepper](http://www.airspayce.com/mikem/arduino/AccelStepper/) web page, and we have included the library within the EX-Turntable software so you don't need to find or download it.

**NmraDcc.h credit:** Also, while not directly used in this software, Alex Shephard's "DCCInterface_TurntableControl" was the inspiration for the initial turntable logic for another DCC driven turntable that translated into the beginnings of EX-Turntable. You can see this code as part of the [NmraDcc Arduino library](https://github.com/mrrwa/NmraDcc).

**EX-Turntable this is the original EX-Turntable source by Pete Cole.  This program uses some sections of that.

EX-DCC-Turntable is a DCC turntable controller, using an Arduino microcontroller to drive a stepper controller and motor to rotate a turntable bridge or drive a horizontal or vertical traverser.


- Interactive commands via the serial console for debugging and testing
- DCC signal phase switching to align bridge track phase with layout phase
- Operates in either turntable or traverser mode
- Operates as a group of turnouts.  This to be rectified when DCC-EX corrects the addressing error in Turntables.cpp


