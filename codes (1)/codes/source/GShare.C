#include "GShare.h"

/*
    G-Share: a global history register in GHR_length bits. Low address_length
             bits of PC as branch address. address_length-bit value
             (GHR << (address_length - GHR_length)) XOR PC[address_length - 1:0]
             is used to locate in pattern history table PHT.
             Each saturating predictor in pattern history table is 2-bit.
*/

/*
    Constructor for G-Share.
*/
GShare::GShare(unsigned int GHR_length, unsigned int address_length) {
    GShare::GHR_length = GHR_length;
    GShare::address_length = address_length;
}

/*
    Initialization for G-Share.
*/
void GShare::init() {
    /*
        G-Share: GHR is initialized with all 0 (all not taken in history).
                     PHT is initialized with all 0 (strongly not taken)
    */
    int i;
    // printf("Initialize G-Share\n");
    GHR = 0;
    for (i = 0; i < (1<<address_length); i++) {
        PHT.push_back(0);
    }
    // printf("Consumed storage: %d bits\n", GHR_length + (1 << address_length) * 2);
    printf("%d,", GHR_length + (1 << address_length) * 2);
}

/*
    Make prediction using G-Share.
*/
bool GShare::make(unsigned int pc) {
    unsigned int address, history, predictor, combined;
    address = pc & (unsigned int)((1<<address_length) - 1); /* Get low address_length-bit address. */
    history = GHR & (unsigned int)((1<<GHR_length) - 1); /* Get global branch history in low GHR_length bits. */
    combined = address ^ (history << (address_length - GHR_length));  /* Get combined position to locate in PHT. */
    predictor = PHT[combined] & 0x3;                 /* Get 2-bit saturating predictor. */
    if (predictor >= 2)                 /* Predict taken if predictor is 2 or 3. */
        return true;
    else                                /* Predict not taken if predictor is 0 or 1. */
        return false;
}

/*
    Train predictor using G-Share.
*/
void GShare::train(unsigned int pc, bool outcome)
{
    unsigned int address, history, predictor, combined;
    address = pc & (unsigned int)((1<<address_length) - 1);      /* Get low GHR_length-bit address. */
    history = GHR & (unsigned int)((1<<GHR_length) - 1);  /* Get history of current branch in low address_length bits. */
    combined = address ^ (history << (address_length - GHR_length)); /* Get combined position to locate in PHT. */
    predictor = PHT[combined] & 0x3;                  /* Get 2-bit saturating predictor. */
    if (outcome == true) {
        if (predictor != 3)
            predictor++;
    } else {
        if (predictor != 0)
            predictor--;
    }
    PHT[combined] = predictor & 0x3;                  /* Update 2-bit saturating predictor. */
    if (outcome == true)
        history = ((history << 1) | 1) & (unsigned int)((1<<GHR_length) - 1); /* Shift to left and add current branch real outcome. */
    else
        history = ((history << 1) | 0) & (unsigned int)((1<<GHR_length) - 1);
    GHR = history & (unsigned int)((1<<GHR_length) - 1);       /* Update history in low GHR_length bits. */
}
