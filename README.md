# JuraDispEmu
Display emulator for Jura J7 dot-matrix display using arduino

Displays content of Jura J7 LED dot-matrix display (59x12) on SS1312 128x32 OLED display.
It can be easy modified for other OLED display controllers.
Right now, it scales up coffee machine display output by the factor of 2 to fill up OLED display.
Arduino pro mini is used for this project, could be easily modified for other dev boards.
Pinout can be seen in comments.
Coffee machine uses 9 74HC595 shift registers to store bitstream of the current row to be displayed (1 1/2 of 9 '595s are used for row selection, others are for column selection).
