#ifndef _TRANS_H_
#define _TRANS_H_

void init_trans(void);
void trans_transmit(uint8_t message);
uint8_t trans_reseive(void);
uint8_t read_reg(uint8_t addr);

#endif