#include<stdio.h>

int getFirstBit(unsigned char);
int getLastBit(unsigned char);
void addRoundKey(unsigned char **);
void subBytesRow(unsigned char *arr, int n);
void invSubBytesRow(unsigned char *arr, int n);
void subBytesState();
void invSubBytesState();
void shiftRow(unsigned char *arr, int n);
void shiftRows();
void inverseShiftRows();
unsigned char GMul(unsigned char a, unsigned char b);
void gmix_word(unsigned char *r);
void inverse_gmix_word(unsigned char *r);
void mixColumns();
void invMixColumns();
void getWord(unsigned char **, int , unsigned char *);
int getKey();
void createRoundKeys();
void initState();
void disposeState();
void disposeRoundKeys();
