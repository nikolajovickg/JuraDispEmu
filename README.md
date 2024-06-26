# JuraDispEmu
Display emulator for Jura J7 dot-matrix display using arduino

Displays content of Jura J7 LED dot-matrix display (59x12) on SSD1312 128x32 OLED display.
It can be easy modified for other OLED display controllers.
Right now, it scales up coffee machine display output by the factor of 2 to fill up OLED display.
Arduino pro mini is used for this project, could be easily modified for other dev boards.
Pinout can be seen in comments.
Coffee machine uses 9 74HC595 shift registers to store bitstream of the current row to be displayed (1 1/2 of 9 '595s are used for row selection, others are for column selection).
In current state, needs u8g2 lib.
Since we're reading pin inputs with software instead of hardware SPI (because of awkward 71-bit bit-stream length from the machine), timing is very critical (frequency is ~90kHz). We're collecting single frame worth of data and display it immidiately, after which we repeat the process.
Current (as of april 10th 2024) U8G2 version doesn't support 128x32 SSD1312, I've used constructor for 128x64, and made 16px y offset. Added 4 pixels more of y offset to center the image.
![Display1](https://github.com/nikolajovickg/JuraDispEmu/assets/8313782/c548102c-9a04-48ad-88c0-1de94d061c3b)
![Display2](https://github.com/nikolajovickg/JuraDispEmu/assets/8313782/31543d30-4a98-4ef8-897e-5ab858060893)

Wiring schematic:
![jura](https://github.com/nikolajovickg/JuraDispEmu/assets/8313782/c79c595a-cb53-4361-b5a1-34a47bc645cb)



