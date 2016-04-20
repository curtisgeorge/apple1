#ifndef __netlist___502__
#define __netlist___502__

#include <stdio.h>
#include <cstdint>
#include "chipsim.h"

class _6502 {
public:
    _6502();
    void step(bool);
    uint16_t readAddressBus();
    uint8_t readDataBus();
    void writeDataBus(uint8_t);
    bool getRW();
    bool getSync();
    void setReset(bool);
    void setSO(bool);
private:
    void initChip();
    void setNode(int, bool);
    
    chipsim sim;
    
    static const int ngnd;
    static const int npwr;
    static const int res;
    static const int clk0;
    static const int rdy;
    static const int so;
    static const int nmi;
    static const int irq;
    static const int rw;
    static const int sync;
};

#endif /* defined(__netlist___502__) */
