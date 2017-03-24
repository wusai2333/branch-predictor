#ifndef ALPHA_H
#define ALPHA_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Predictor_base.h"

using namespace std;

/*
    Alpha 21264: a choice register table CRT contains 2 ^ GHR_length 2-bit counter.
                A global prediction table GPT contains 2 ^ GHR_length 2-bit saturating
                predictors. A global history register GHR contains GHR_length-bit
                global history. A local history table LHT uses low
                address_length bits of PC to index, and each entry contains a
                LHR_length-bit local history register. A local prediction table LPT
                contains 2 ^ LHR_length 3-bit saturating predictors.
*/

class Alpha : public Predictor {
    unsigned int GHR_length, address_length, LHR_length;
    unsigned int GHR;
    vector <unsigned int> CRT, GPT,
                        LHT, LPT;

public:
    Alpha(unsigned int GHR_length, unsigned int address_length,
          unsigned int LHR_length);
    void init();
    bool make(unsigned int pc);
    void train(unsigned int pc, bool outcome);
};

#endif
