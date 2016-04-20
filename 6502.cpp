#include "6502.h"
#include "segdefs_6502.h"
#include "transdefs_6502.h"
#include <bitset>
#include <assert.h>

const int _6502::ngnd = 558;
const int _6502::npwr = 657;
const int _6502::res = 159;
const int _6502::clk0 = 1171;
const int _6502::rdy = 89;
const int _6502::so = 1672;
const int _6502::nmi = 1297;
const int _6502::irq = 103;
const int _6502::rw = 1156;
const int _6502::sync = 539;

_6502::_6502() : sim(segdefs_6502, transdefs_6502, ngnd, npwr) {
    initChip();
}

void _6502::initChip() {
    for(auto i = 0; i < sim.segdefs.size(); i++) {
        if(sim.nodes[i] != NULL) {
            sim.nodes[i]->state = false;
        }
    }
    sim.nodes[ngnd]->state = false;
    sim.nodes[npwr]->state = true;
    for(auto i = 0 ; i < sim.transdefs.size(); i++) {
        sim.transistors[i]->on = false;
    }
    
    sim.setHigh(res);
    sim.setLow(clk0);
    sim.setHigh(rdy); sim.setLow(so);
    sim.setHigh(irq); sim.setHigh(nmi);
    sim.recalcNodeList(sim.allNodes());
}

void _6502::step(bool clk) {
    setNode(clk0, clk);
}

uint16_t _6502::readAddressBus() {
    return sim.isNodeHigh(195) << 15 |
           sim.isNodeHigh(672) << 14 |
           sim.isNodeHigh(349) << 13 |
           sim.isNodeHigh(1237) << 12 |
           sim.isNodeHigh(399) << 11 |
           sim.isNodeHigh(1443) << 10 |
           sim.isNodeHigh(148) << 9 |
           sim.isNodeHigh(230) << 8 |
           sim.isNodeHigh(1493) << 7 |
           sim.isNodeHigh(887) << 6 |
           sim.isNodeHigh(736) << 5 |
           sim.isNodeHigh(435) << 4 |
           sim.isNodeHigh(211) << 3 |
           sim.isNodeHigh(1340) << 2 |
           sim.isNodeHigh(451) << 1 |
           sim.isNodeHigh(268);
}

uint8_t _6502::readDataBus(){
    return sim.isNodeHigh(1349) << 7 |
           sim.isNodeHigh(1591) << 6 |
           sim.isNodeHigh(175) << 5 |
           sim.isNodeHigh(1393) << 4 |
           sim.isNodeHigh(650) << 3 |
           sim.isNodeHigh(945) << 2 |
           sim.isNodeHigh(82) << 1 |
           sim.isNodeHigh(1005);
}

void _6502::writeDataBus(uint8_t x){
    std::vector<int> recalcs;
    int dbnodes[8] = {1005, 82, 945, 650, 1393, 175, 1591, 1349};
    for(auto i=0;i<8;i++){
        int nn = dbnodes[i];
        node* n = sim.nodes[nn];
        if((x%2)==0) {
            n->pulldown=true; n->pullup=false;
        }
        else {
            n->pulldown=false; n->pullup=true;
        }
        recalcs.push_back(nn);
        x>>=1;
    }
    sim.recalcNodeList(recalcs);
}

bool _6502::getRW() {
    return sim.isNodeHigh(rw);
}

bool _6502::getSync() {
    return sim.isNodeHigh(sync);
}

void _6502::setReset(bool val) {
    setNode(res, val);
}

void _6502::setSO(bool val) {
    setNode(so, val);
}

void _6502::setNode(int node, bool state) {
    if(state) {
        sim.setHigh(node);
    }
    else {
        sim.setLow(node);
    }
}
