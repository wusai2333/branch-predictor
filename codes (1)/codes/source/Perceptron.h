#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Predictor_base.h"

using namespace std;

/*
    Perceptron: a global history register in GHR_length bits (x0 is always 1).
                Low address_length bits of PC is used to locate perceptron
                table PT. 2^address_length entries in PT, each one is
                w_count * w_length bits, that is, w_count-dimension vector and
                each dimension is an w_length bits signed number.
*/

class Perceptron : public Predictor {
    unsigned int theta, GHR_length, address_length, w_count, w_length;
    unsigned int GHR;
    vector<vector<signed int> > PT;

public:
    Perceptron(unsigned theta, unsigned int GHR_length,
        unsigned int address_length, unsigned int w_count,
        unsigned int w_length);
    void init();
    bool make(unsigned int pc);
    void train(unsigned int pc, bool outcome);

private:
    int abs(int num);
    signed int get_signed_bits(signed int num, int bits);
};

#endif
