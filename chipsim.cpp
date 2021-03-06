#include <algorithm>
#include "chipsim.h"

chipsim::chipsim(std::vector<int>& segdefs, std::vector< std::vector<int> >& transdefs, int ngnd, int npwr) :  segdefs(segdefs), transdefs(transdefs), ngnd(ngnd), npwr(npwr)
{
    transistors.resize(transdefs.size());
    nodes.resize(segdefs.size());
    recalcHash.resize(segdefs.size());
    setupNodes();
    setupTransistors();
}

void chipsim::setupNodes() {
    for(auto i = 0; i < segdefs.size(); i++) {
        if(segdefs[i] != 2) {
            nodes[i] = std::make_shared<node>();
            nodes[i]->pullup = segdefs[i];
            nodes[i]->state = false;
        }
        else {
            nodes[i] = std::shared_ptr<node>(nullptr);
        }
    }
}

void chipsim::setupTransistors() {
    for(auto i = 0; i < transdefs.size(); i++) {
        std::vector<int> tdef = transdefs[i];
        int gate = tdef[0];
        int c1 = tdef[1];
        int c2 = tdef[2];
        if(c1==ngnd){c1=c2; c2=ngnd;}
        if(c1==npwr){c1=c2; c2=npwr;}
        auto trans = std::make_shared<transistor>();
        trans->on = false;
        trans->gate = gate;
        trans->c1 = c1;
        trans->c2 = c2;
        nodes[gate]->gates.push_back(trans);
        nodes[c1]->c1c2s.push_back(trans);
        nodes[c2]->c1c2s.push_back(trans);
        transistors[i] = trans;
    }
}

void chipsim::setHigh(int nn){
    nodes[nn]->pullup = true;
    nodes[nn]->pulldown = false;
    std::vector<int> list;
    list.push_back(nn);
    recalcNodeList(list);
}

void chipsim::setLow(int nn){
    nodes[nn]->pullup = false;
    nodes[nn]->pulldown = true;
    std::vector<int> list;
    list.push_back(nn);
    recalcNodeList(list);
}

bool chipsim::isNodeHigh(int nn) {
    return(nodes[nn]->state);
}

void chipsim::recalcNodeList(std::vector<int>& list){
    recalclist.clear();
    recalcHash.clear();
    recalcHash.resize(segdefs.size());
    for(auto j=0;j<100;j++){		// loop limiter
        if(list.size()==0) return;
        for(int i : list) {
            recalcNode(i);
        }
        list = recalclist;
        recalclist.clear();
        recalcHash.clear();
        recalcHash.resize(segdefs.size());
    }
}

void chipsim::recalcNode(int nn){
    if(nn==ngnd) return;
    if(nn==npwr) return;
    getNodeGroup(nn);
    bool newState = getNodeValue();
    for(int i : group) {
        auto n = nodes[i];
        if(n->state==newState) continue;
        n->state = newState;
        for(auto t : n->gates) {
            if(n->state){
                turnTransistorOn(t);
            }
            else {
                turnTransistorOff(t);
            }
        }
    }
}

void chipsim::getNodeGroup(int i){
    group.clear();
    addNodeToGroup(i);
}

void chipsim::addRecalcNode(int nn){
    if(nn==ngnd) return;
    if(nn==npwr) return;
    if(recalcHash[nn] == 1)return;
    recalclist.push_back(nn);
    recalcHash[nn] = 1;
}

bool chipsim::arrayContains(std::vector<int>& array, int n) {
    auto it=std::find(array.begin(),array.end(),n);
    return (it != array.end());
}

bool chipsim::getNodeValue(){
    if(arrayContains(group, ngnd)) return false;
    if(arrayContains(group, npwr)) return true;
    for(int i : group){
        auto n = nodes[i];
        if(n->pullup) return true;
        if(n->pulldown) return false;
        if(n->state) return true;
    }
    return false;
}

void chipsim::turnTransistorOn(std::shared_ptr<transistor>& t){
    if(t->on) return;
    t->on = true;
    addRecalcNode(t->c1);
}

void chipsim::turnTransistorOff(std::shared_ptr<transistor>& t){
    if(!t->on) return;
    t->on = false;
    addRecalcNode(t->c1);
    addRecalcNode(t->c2);
}

void chipsim::addNodeToGroup(int i){
    auto it=std::find(group.begin(),group.end(),i);
    if(it != group.end()) return;
    group.push_back(i);
    if(i==ngnd) return;
    if(i==npwr) return;
    for(auto t : nodes[i]->c1c2s) {
        if(!t->on) continue;
        int other;
        if(t->c1==i) other=t->c2;
        if(t->c2==i) other=t->c1;
        addNodeToGroup(other);
    }
}

std::vector<int> chipsim::allNodes(){
    std::vector<int> res;
    for(int i = 0 ; i < segdefs.size(); i++) {
        if(nodes[i] != nullptr) {
            if((i!=npwr)&&(i!=ngnd)) res.push_back(i);
        }
    }
    return res;
}
