#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include <fstream>
#include <bitset>
#include "6502.h"

uint8_t bank0[4096];
uint8_t bank1[4096];
uint8_t bankE[4096];
uint8_t bankF[256];
uint8_t DRA = 0;
uint8_t DRB = 0;
//uint8_t DDRA = 0;
uint8_t DDRB = 0;
std::bitset<8> CRA = 0;
std::bitset<8> CRB = 0;

int main() {
  std::ifstream rom;
  rom.open("apple1.rom", std::ios::in | std::ios::binary);
  rom.read((char *) bankF, 256);
  rom.close();
    
  std::ifstream basic;
  basic.open("apple1basic.bin", std::ios::in | std::ios::binary);
  basic.read((char *) bankE, 4096);
  basic.close();

  _6502 cpu;
  initscr();
  //curs_set(0);
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  wresize(stdscr, 24, 40);
  scrollok(stdscr, TRUE); 
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));

  bool clk = false;
  cpu.setReset(false);
  for(auto i = 0; i < 16; i++) {
    clk = !clk;
    cpu.step(clk);
  }
  cpu.setReset(true);

  refresh();
  while(true) {
    clk = !clk;
    char c = getch();
    if(c != ERR) {
      if(c == 10) {
        c = 13;
      }
      DRA = toupper(c) | 0x80;
      CRA[7] = 1;
    }
    if(CRB[7]) {
      c = DRB & DDRB;
      if(c == 13) {
        c = 10;
      }
      printw("%c", c);
      refresh();
      CRB[7] = 0;
    }
    cpu.step(clk);
    uint16_t address = cpu.readAddressBus();
    uint8_t bank = address >> 12;
    if(clk && !cpu.getRW()) {
      address = address & 0x0FFF;
      uint8_t data = cpu.readDataBus();
      uint8_t port;
      switch(bank) {
        case 0x0:
          bank0[address] = data;
          break;
        case 0x1:
          bank1[address] = data;
          break;
        case 0xD:
          port = address & 0x0013;
          switch(port) {
            case 0x11:
              CRA = data;
              break;
            case 0x12:
              if(CRB[2]) {
                DRB = data & 0x7F;
                CRB[7] = 1;
              }
              else {
                DDRB = data;
              }
              break;
            case 0x13:
              CRB = data;
              break;
          }
          break;
        case 0xE:
          bankE[address] = data;
          break;
      }
    }
    else if(!clk && cpu.getRW()) {
      address = address & 0x0FFF;
      uint8_t data;
      uint8_t port;
      char c;
      switch(bank) {
        case 0x0:
          data = bank0[address];
          break;
        case 0x1:
          data = bank1[address];
          break;
        case 0xD:
          port = address & 0x0013;
          switch(port) {
            case 0x10:
              data = DRA;
              CRA[7] = 0;
              break;
            case 0x11:
              data = CRA.to_ulong();
              break;
            case 0x13:
              data = CRB.to_ulong();
              break;
            default:
              data = 0x00;
              break;
          }
          break;
        case 0xE:
          data = bankE[address];
          break;
        case 0xF:
          address = address & 0x00FF;
          data = bankF[address];
          break;
        default:
          data = 0x00;
          break;
      }
      cpu.writeDataBus(data);
    }
  }
  nocbreak();
  nodelay(stdscr, FALSE);
  endwin();

  return 0;
}
