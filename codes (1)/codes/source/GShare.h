#ifndef GSHARE_H
#define GSHARE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Predictor_base.h"

using namespace std;

/*
    G-Share: a global history register in GHR_length bits. Low address_length
             bits of PC as branch address. address_length-bit value
             (GHR << (address_length - GHR_length)) XOR PC[address_length - 1:0]
             is used to locate in pattern history table PHT.
             Each saturating predictor in pattern history table is 2-bit.
*/

class GShare : public Predictor {
    unsigned int GHR_length, address_length;
    unsigned int GHR;
    vector<unsigned int> PHT;

public:
    GShare(unsigned int GHR_length, unsigned int address_length);
    void init();
    bool make(unsigned int pc);
    void train(unsigned int pc, bool outcome);
};

#endif
