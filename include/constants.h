extern unsigned char **state;
extern unsigned char key[256/8 + 1];

extern int mode;   // 0: 128 bit, 1: 192 bit, 2: 256 bit

extern unsigned char ***roundKeys;

extern const unsigned char sbox[16][16];

extern const unsigned char inv_sbox[16][16];

extern const unsigned char rcon[255];