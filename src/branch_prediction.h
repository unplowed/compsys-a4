#pragma once
#include <stdint.h>

#define GHR_BITS 8 // GHR = Global History Register
#define GHR_MASK ((1 << GHR_BITS) -1)

typedef enum {
    None = 0,
    NT,
    BTFNT,
    Bimodal,
    gShare
} predictor_mode;

typedef struct branch_prediction_data {
  unsigned int GlobalHistoryRegister;
  uint8_t *BranchHistoryTable;
  int BranchHistoryTableSize;
} branch_prediction_data_t;

void init_BHT(branch_prediction_data_t *data, int size);
int branch_prediction(branch_prediction_data_t *data, int pc, int offset, predictor_mode predictor);
