#include "Perceptron.h"

/*
    Perceptron: a global history register in GHR_length bits (x0 is always 1).
                Low address_length bits of PC is used to locate perceptron
                table PT. 2^address_length entries in PT, each one is
                w_count * w_length bits, that is, w_count-dimension vector and
                each dimension is an w_length bits signed number.
*/

/*
    Constructor for Perceptron.
*/
Perceptron::Perceptron(unsigned theta, unsigned int GHR_length,
    unsigned int address_length, unsigned int w_count,
    unsigned int w_length) {
        Perceptron::theta = theta;
        Perceptron::GHR_length = GHR_length;
        Perceptron::address_length = address_length;
        Perceptron::w_count = w_count;
        Perceptron::w_length = w_length;
}

/*
    Get absolute value.
*/
int Perceptron::abs(int num) {
    if (num < 0)
        return -num;
    else
        return num;
}

/*
    Get signed bits from LSB.
*/
signed int Perceptron::get_signed_bits(signed int num, int bits) {
    unsigned int sign = ((unsigned int)num) & (1 << (bits - 1));
    unsigned padding;
    if (sign != 0) {                     /* Minus sign exists. */
        padding = (1 << bits) - 1;
        padding = ~padding;
        return (signed int)(((unsigned int)num) | padding);
    } else {                             /* Minus sign does not exist. */
        padding = (1 << bits) - 1;
        return (signed int)(((unsigned int)num) & padding);
    }
}

/*
    Initialization for Perceptron.
*/
void Perceptron::init(){
    /*
        Perceptron: GHR is intialized with all 0 in GHR_length bits (all not taken in history).
                    PT is intialized with all 0.
    */
    int i, j;
    // printf("Initialize Perceptron\n");
    GHR = 0;
    for (i = 0; i < (1<<address_length); i++) {
        vector<signed int> v; /* FIXME: shallow copy? */
        for (j = 0; j < w_count; j++)
            v.push_back(0);
        PT.push_back(v);
    }
    // printf("Consumed storage: %d bits\n", (GHR_length + (1<<address_length)
    //     * w_count * w_length));
    printf("%d,", (GHR_length + (1<<address_length)
        * w_count * w_length));
}

/*
    Make prediction using Perceptron.
*/
bool Perceptron::make(unsigned int pc) {
    unsigned int address, history, i;
    signed int w, y_out;
    address = pc & (unsigned int)((1<<address_length) - 1); /* Get low address_length-bit address. */
    history = GHR & (unsigned int)((1<<GHR_length) - 1);    /* Get GHR_length bits of history. bit 0 is x1, bit GHR_length - 1 is xGHR_length. */
    y_out = PT[address][0];                                 /* Initialize as bias w0. */
    for (i = 1; i < w_count; i++)                           /* FIXME: mixed constraint. */
        if ((history & (unsigned int)(1<<(i - 1))) == 0)    /* Not taken in history xi (i from 1 to w_count - 1). */
            y_out -= PT[address][i];                        /* Minus wi (i from 1 to w_count - 1), wi is low w_length bits. */
        else
            y_out += PT[address][i];                        /* Add wi (i from 1 to w_count - 1), wi is low w_length bits. */
    if (y_out < 0)                      /* y_out < 0: not taken. */
        return false;
    else                                /* y_out >= 0: taken. */
        return true;
}

/*
    Train predictor using Perceptron.
*/
void Perceptron::train(unsigned int pc, bool outcome) {
    unsigned int address, history, i;
    signed int w, y_out;
    address = pc & (unsigned int)((1<<address_length) - 1); /* Get low address_length-bit address. */
    history = GHR & (unsigned int)((1<<GHR_length) - 1);    /* Get GHR_length bits of history. bit 0 is x1, bit (GHR_length - 1) is xGHR_length. */
    y_out = PT[address][0];                                 /* Initialize as bias w0. */
    for (i = 1; i < w_count; i++)
        if ((history & (unsigned int)(1<<(i - 1))) == 0)    /* Not taken in history xi (i from 1 to w_count - 1). */
            y_out -= PT[address][i];                        /* Minus wi (i from 1 to w_count - 1), wi is low w_length bits. */
        else
            y_out += PT[address][i];                        /* Add wi (i from 1 to w_count - 1), wi is low w_length bits. */
    if (((y_out < 0) && (outcome == true)) ||               /* Prediction incorrect (predict not taken, actually taken). */
        ((y_out >= 0) && (outcome == false)) ||             /* Prediction incorrect (predict taken, actually not taken). */
        abs(y_out) <= theta) {                              /* Not enough accurancy. */
        if (outcome == true) {                              /* Update w0. */
             if (PT[address][0] != ((1<<(w_length - 1)) - 1))
                PT[address][0] = PT[address][0] + 1;        /* If actually taken, new w0 = w0 + tx0 = w0 + x0 = w0 + 1. */
        } else {
             if (PT[address][0] != -(1<<(w_length - 1)))
                PT[address][0] = PT[address][0] - 1;        /* If actually not taken, new w0 = w0 + tx0 = w0 - x0 = w0 - 1. */
        }
        for (i = 1; i < w_count; i++) {                     /* Update wi (i from 1 to w_count - 1). */
            if (((outcome == true) &&
                ((history & (unsigned int)(1 << (i - 1))) == 1)) ||
                ((outcome == false) &&
                ((history & (unsigned int)(1 << (i - 1))) == 0))) {
                 if (PT[address][i] != ((1<<(w_length - 1)) - 1))
                    PT[address][i] = PT[address][i] + 1;    /* New wi = wi + txi = wi + 1. */
            } else {
                 if (PT[address][i] != -(1<<(w_length - 1)))
                    PT[address][i] = PT[address][i] - 1;    /* New wi = wi + txi = wi - 1. */
            }
        }
    }
    /*
        Update GHR.
    */
    if (outcome == true)
        history = ((history << 1) | 1) & (unsigned int)((1<<GHR_length) - 1);    /* Shift to left and add current branch real outcome. */
    else
        history = ((history << 1) | 0) & (unsigned int)((1<<GHR_length) - 1);
    GHR = history & (unsigned int)((1<<GHR_length) - 1);    /* Update history in low GHR_length bits. */
}
