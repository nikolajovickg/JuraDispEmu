// PIN2 - Clock
// PIN7 - Data
// PIN3 - Load
// PIN4 - COL59
#define CLK 0b00000100
#define DATA 0b10000000
#define LOAD 0b00010000
#define COL59 0b00001000
#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
bool oldclk = false;
uint8_t state = 0;
uint8_t clkNum = 0;
uint8_t byteIndex = 0;
char disp[96]; //display memory
inline void state0() __attribute__((always_inline));
inline void state1() __attribute__((always_inline));
inline void state2() __attribute__((always_inline));
inline void state3() __attribute__((always_inline));
inline void state4() __attribute__((always_inline));
inline void state5() __attribute__((always_inline));
void setup() {
  for (int i = 0; i < 96; i++) {
    disp[i] = 0x00;
  }
  pinMode(4, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  u8g2.begin();
  // disable timer0, don't need it, ISR for millis() steals some time
  _SFR_BYTE(TIMSK0) &= ~_BV(TOIE0);
  noInterrupts();
  for (;;) {
    switch (state) {
      case 0:
        state0();
        break;
      case 1:
        state1();
        break;
      case 2:
        state2();
        break;
      case 3:
        state3();
        break;
      case 4:
        state4();
        break;
      case 5:
        state5();
        break;
    }
  }

}


// new display frame
void state0() {
  while (true)
    if (PIND & LOAD) {
      state = 1;
      clkNum = 0;
      oldclk = 0;
      break;
    }
}

// first row
void state1() {
  while (true) {
    uint8_t clk = PIND & CLK;
    if (clk && !oldclk) {
      clkNum++;
      if (PIND & DATA) {
        oldclk = clk;
        if (clkNum == 12) {
          state = 2;
          clkNum = 0;
          byteIndex = 0;
          break;
        } else {
          state = 0;
          break;
        }
      }
    }
    oldclk = clk;
  }
}
// read pixels in row
void state2() {
  while (true) {
    uint8_t clk = PIND & CLK;
    if (clk && !oldclk) {
      // DATA wire moved to D7 in order to skip bit-shifting (can optimize 62.5nS)
      disp[byteIndex + (clkNum >> 3)] |= ((PIND & DATA)) >> (clkNum & 0b00000111);
      clkNum++;
    }
    if (PIND & LOAD) {
      disp[byteIndex + (clkNum >> 3)] |= ~((~COL59 | (PIND & COL59))) << 2;
      clkNum = 0;
      oldclk = clk;
      if (byteIndex == 88) {
        state = 4;
        break;
      } else {
        state = 3;
        break;
      }
    }
    oldclk = clk;
  }
}

// other rows
void state3() {
  while (true) {
    uint8_t clk = PIND & CLK;
    if (clk && !oldclk) {
      clkNum++;
      if ((PIND & DATA)) {
        oldclk = clk;
        state = 5;
        byteIndex = (12 - clkNum) << 3;
        break;
      }

    }
    oldclk = clk;
  }
}

// enough data for display
void state4() {
  interrupts();
  u8g2.clearBuffer();
  u8g2.drawBitmap(20, 10, 8, 12, disp);
  u8g2.sendBuffer();
  noInterrupts();
  for (uint8_t i = 0; i < 96; i++) {
    disp[i] = 0;
  }
  state = 0;
  clkNum = 0;
  oldclk = PIND & CLK;
}

// pause until end of row number payload
void state5() {
  while (true) {
    uint8_t clk = PIND & CLK;
    if (clk && !oldclk) {
      clkNum ++;
      if (clkNum == 12) {
        oldclk = clk;
        state = 2;
        clkNum = 0;
        break;
      }
    }
    oldclk = clk;
  }
}

void loop() {

}
