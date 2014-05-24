#ifndef UTILS_H
#define UTILS_H

void blink(int j);
void bit_set(int8_t *coil, int pos);
void bit_clear(int8_t *coil, int pos);
bit bit_test(int8_t coil, int pos);
char swap_bits(char c);
#define make16(msb, lsb)        ((msb << 8) | lsb)
#define bit_test(var,pos)       ((var) & (1<<(pos)))
#define bit_set(coil,pos)       (coil |= 1 << pos)
#define bit_clear(coil, pos)    (coil &= ~(1 << pos))
#define swap_bits(c)            ((c&1)?128:0)|((c&2)?64:0)|((c&4)?32:0)|((c&8)?16:0 )|((c&16)?8:0)|((c&32)?4:0)|((c&64)?2:0)|((c&128)?1:0)





#endif