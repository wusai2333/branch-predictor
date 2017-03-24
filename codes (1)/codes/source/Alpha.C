#include "Alpha.h"

/*
    Alpha 21264: a choice register table CRT contains 2 ^ GHR_length 2-bit counter.
                A global prediction table GPT contains 2 ^ GHR_length 2-bit saturating
                predictors. A global history register GHR contains GHR_length-bit
                global history. A local history table LHT uses low
                address_length bits of PC to index, and each entry contains a
                LHR_length-bit local history register. A local prediction table LPT
                contains 2 ^ LHR_length 3-bit saturating predictors.
*/
 
/*
    Constructor for Alpha 21264.
*/
Alpha::Alpha(unsigned int GHR_length, unsigned int address_length,
      unsigned int LHR_length) {
    Alpha::GHR_length = GHR_length;
    Alpha::address_length = address_length;
    Alpha::LHR_length = LHR_length;
}

/* 
    Initialization for Alpha 21264.
*/
void Alpha::init() {
    /*
        Alpha 21264: GHR is initialized with all 0 (all not taken in history).
                     CRT is initialized with all 0 (strongly choose local).
                     GPT is initialized with all 0 (strongly not taken).
                     LHT is initialized with all 0 (all not taken in history).
                     LPT is initialized with all 0 (strongly not taken).
    */
    int i;
    // printf("Initialize Alpha 21264\n");
    GHR = 0;
    for (i = 0; i < (1<<GHR_length); i++) {
        CRT.push_back(0);
        GPT.push_back(0);
    }
    for (i = 0; i < (1<<address_length); i++) {
        LHT.push_back(0);
    }
    for (i = 0; i < (1<<LHR_length); i++) {
        LPT.push_back(0);
    }
    // printf("Consumed storage: %d bits\n", GHR_length + (1 << GHR_length) *
    //     (2 + 2) + (1 << address_length) * LHR_length + (1 << LHR_length) * 3);
    printf("%d,", GHR_length + (1 << GHR_length) *
        (2 + 2) + (1 << address_length) * LHR_length + (1 << LHR_length) * 3);
}

/*
    Make prediction using Alpha 21264.
*/
bool Alpha::make(unsigned int pc) {
    unsigned int address, local_history, local_predictor,
                 global_history, global_predictor, choice;
    bool local_result, global_result;
    address = pc & (unsigned int)((1<<address_length) - 1);             /* Get low address_length-bit address. */
    local_history = LHT[address] & (unsigned int)((1<<LHR_length) - 1); /* Get local history of current branch in low LHR_length bits. */
    local_predictor = LPT[local_history] & 0x7;                         /* Get 3-bit local saturating predictor. */
    if (local_predictor >= 4)           /* Predict taken if local predictor is 4, 5, 6 or 7. */
        local_result = true;
    else                                /* Predict not taken if local predictor is 0, 1, 2 or 3. */
        local_result = false;
    global_history = GHR & (unsigned int)((1<<GHR_length) - 1);         /* Get GHR_length-bit global branch history. */
    global_predictor = GPT[global_history] & 0x3;                       /* Get 2-bit global saturating predictor. */
    if (global_predictor >= 2)          /* Predict taken if global predictor is 2 or 3. */
        global_result = true;
    else                                /* Predict not taken if global predictor is 0 or 1. */
        global_result = false;
    choice = CRT[global_history] & 0x3;                           /* Get 2-bit choice register. */
    if (choice <= 1)                    /* Use local predictor if choice register is 0 or 1. */
        return local_result;
    else                                /* Use global predictor if choice register is 2 or 3. */
        return global_result;
}

/*
    Train predictor using Alpha 21264.
*/
void Alpha::train(unsigned int pc, bool outcome)
{
    unsigned int address, local_history, local_predictor,
                 global_history, global_predictor, choice;
    bool local_result, global_result;
    address = pc & (unsigned int)((1<<address_length) - 1);             /* Get low address_length-bit address. */
    local_history = LHT[address] & (unsigned int)((1<<LHR_length) - 1); /* Get local history of current branch in low LHR_length bits. */
    local_predictor = LPT[local_history] & 0x7;                         /* Get 3-bit local saturating predictor. */
    if (local_predictor >= 4)           /* Predict taken if local predictor is 4, 5, 6 or 7. */
        local_result = true;
    else                                /* Predict not taken if local predictor is 0, 1, 2 or 3. */
        local_result = false;
    global_history = GHR & (unsigned int)((1<<GHR_length) - 1);         /* Get GHR_length-bit global branch history. */
    global_predictor = GPT[global_history] & 0x3;                       /* Get 2-bit global saturating predictor. */
    if (global_predictor >= 2)          /* Predict taken if global predictor is 2 or 3. */
        global_result = true;
    else                                /* Predict not taken if global predictor is 0 or 1. */
        global_result = false;
    choice = CRT[global_history] & 0x3;                                 /* Get 2-bit choice register. */

    if ((local_result == outcome) && (global_result != outcome)) {      /* If local predictor is correct and global predictor is incorrect. */
        if (choice != 0) {
            choice--;                   /* More likely using local predictor. */
        }
    }
    else if ((local_result != outcome) && (global_result == outcome)) {  /* If local predictor is incorrect and global predictor is correct. */
        if (choice != 3) {
            choice++;                   /* More likely using global predictor. */
        }
    }
    CRT[global_history] = choice & 0x3;                                 /* Update 2-bit choice register. */

    if (outcome == true) {
        if (local_predictor != 7)
            local_predictor++;
    } else {
        if (local_predictor != 0)
            local_predictor--;
    }
    LPT[local_history] = local_predictor & 0x7;                         /* Update 3-bit local saturating predictor. */

    if (outcome == true)
        local_history = ((local_history << 1) | 1) & (unsigned int)((1<<LHR_length) - 1); /* Shift to left and add current branch real outcome. */
    else
        local_history = ((local_history << 1) | 0) & (unsigned int)((1<<LHR_length) - 1);
    LHT[address] = local_history & (unsigned int)((1<<LHR_length) - 1);   /* Update local history in low LHR_length bits. */

    if (outcome == true) {
        if (global_predictor != 3)
            global_predictor++;
    } else {
        if (global_predictor != 0)
            global_predictor--;
    }
    GPT[global_history] = global_predictor & 0x3;                       /* Update 2-bit global saturating predictor. */

    if (outcome == true)
        global_history = ((global_history << 1) | 1) & (unsigned int)((1<<GHR_length) - 1); /* Shift to left and add current branch real outcome. */
    else
        global_history = ((global_history << 1) | 0) & (unsigned int)((1<<GHR_length) - 1);
    GHR = global_history & (unsigned int)((1<<GHR_length) - 1);         /* Update global history in low GHR_length bits. */
}
