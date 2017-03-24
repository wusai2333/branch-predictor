#include "PAgA2.h"

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

/*
    Constructor for PAg with A2.
*/
PAgA2::PAgA2(unsigned int address_length, unsigned int LHR_length) {
    PAgA2::address_length = address_length;
    PAgA2::LHR_length = LHR_length;
}

/*
    Initialization for PAg with A2.
*/
void PAgA2::init() {
    /*
        PAg with A2: PBHT is initialized with all 0 (all not taken in history).
                     GPHT is initialized with all 0 (strongly not taken)
    */
    int i;
    // printf("Initialize PAg with A2\n");
    for (int i = 0; i < (1<<address_length); i++) {
        PBHT.push_back(0);
    }
    for (int i = 0; i < (1<<LHR_length); i++) {
        GPHT.push_back(0);
    }
    // printf("Consumed storage: %d bits\n", (LHR_length * (1<<address_length) +
    //    2 * (1<<LHR_length)));
    printf("%d,", (LHR_length * (1<<address_length) +
       2 * (1<<LHR_length)));
}

/*
    Make prediction using PAg with A2.
*/
bool PAgA2::make(unsigned int pc) {
    unsigned int address, history, predictor;
    address = pc & (unsigned int)((1<<address_length) - 1);   /* Get low address_length-bit address. */
    history = PBHT[address] & (unsigned int)((1<<LHR_length) - 1); /* Get history of current branch in low LHR_length bits. */
    predictor = GPHT[history] & 0x3;                          /* Get 2-bit saturating predictor. */
    if (predictor >= 2)                 /* Predict taken if predictor is 2 or 3. */
        return true;
    else                                /* Predict not taken if predictor is 0 or 1. */
        return false;
}

/*
    Train predictor using PAg with A2.
*/
void PAgA2::train (unsigned int pc, bool outcome)
{
    unsigned int address, history, predictor;
    address = pc & (unsigned int)((1<<address_length) - 1);             /* Get low address_length-bit address. */
    history = PBHT[address] & (unsigned int)((1<<LHR_length) - 1);      /* Get history of current branch in low LHR_length bits. */
    predictor = GPHT[history] & 0x3;    /* Get 2-bit saturating predictor. */
    if (outcome == true) {
        if (predictor != 3)
            predictor++;
    } else {
        if (predictor != 0)
            predictor--;
    }
    GPHT[history] = predictor & 0x3;    /* Update 2-bit saturating predictor. */
    if (outcome == true)
        history = ((history << 1) | 1) & (unsigned int)((1<<LHR_length) - 1); /* Shift to left and add current branch real outcome. */
    else
        history = ((history << 1) | 0) & (unsigned int)((1<<LHR_length) - 1);
    PBHT[address] = history & (unsigned int)((1<<LHR_length) - 1);            /* Update history in low LHR_length bits. */
}
