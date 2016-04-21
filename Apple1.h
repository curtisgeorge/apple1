#ifndef _Apple1_h
#define _Apple1_h

#include "6502.h"
#include <bitset>

class Apple1 {
public:
  Apple1();
  void reset();
  void run();
private:
  bool clk;
  _6502 cpu;
  uint8_t bank0[4096] = {0};
  uint8_t bank1[4096] = {0};
  uint8_t bankE[4096];
  uint8_t bankF[256];
  uint8_t DRA;
  uint8_t DRB;
  uint8_t DDRA;
  uint8_t DDRB;
  std::bitset<8> CRA;
  std::bitset<8> CRB;
};

#endif
