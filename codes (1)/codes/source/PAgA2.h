#ifndef PAGA2_H
#define PAGA2_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Predictor_base.h"

using namespace std;

/*
    PAg with A2: direct mapping using low address_length bits of PC,
                2^address_length entries in PBHT, each one is LHR_length bits
                and the low LHR_length bits of PBHT is used. 2^LHR_length
                entries in GPHT, each one is 2 bits using A2 2-bit
                saturating predictor and the low 2 bits of GPHT is used.
*/
/*
    LHR_length is local history register length. A local history register is an
    entry in PBHT (Per-address Branch History Table).
*/

class PAgA2 : public Predictor {
    unsigned int address_length, LHR_length;
    vector<unsigned int> PBHT, GPHT;

public:
    PAgA2(unsigned int address_length, unsigned int LHR_length);
    void init();
    bool make(unsigned int pc);
    void train(unsigned int pc, bool outcome);
};

#endif
