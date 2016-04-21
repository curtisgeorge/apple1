#ifndef __netlist__chipsim__
#define __netlist__chipsim__

#include <vector>
#include <memory>

class transistor {
public:
    bool on;
    int gate;
    int c1;
    int c2;
};

class node {
public:
    bool pullup;
    bool pulldown;
    bool state;
    std::vector< std::shared_ptr<transistor> > gates;
    std::vector< std::shared_ptr<transistor> > c1c2s;
};

class chipsim {
public:
    chipsim(std::vector<int>&, std::vector< std::vector<int> >&, int, int);
    void setHigh(int);
    void setLow(int);
    bool isNodeHigh(int);
    void recalcNodeList(std::vector<int>&);
    std::vector<int> allNodes();
    std::vector<int>& segdefs;
    std::vector< std::vector<int> >& transdefs;
    std::vector< std::shared_ptr<node> > nodes;
    std::vector< std::shared_ptr<transistor> > transistors;
private:
    void setupNodes();
    void setupTransistors();
    void recalcNode(int);
    void getNodeGroup(int);
    void addRecalcNode(int);
    bool arrayContains(std::vector<int>&, int);
    bool getNodeValue();
    void turnTransistorOn(std::shared_ptr<transistor>&);
    void turnTransistorOff(std::shared_ptr<transistor>&);
    void addNodeToGroup(int);
    
    int ngnd;
    int npwr;
    std::vector<int> group;
    std::vector<int> recalclist;
    std::vector<int> recalcHash;
};

#endif /* defined(__netlist__chipsim__) */
