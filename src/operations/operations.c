#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"constants.h"

int getFirstBit(unsigned char ch){
    return ch >> 4;
}

int getLastBit(unsigned char ch){
    return ch & 0x0F;
}

void getWord(unsigned char **arr, int n, unsigned char *val){ //Works fine
    for(int i = 0; i<4; i++){
        val[i] = arr[i][n];
    }
}

void addRoundKey(unsigned char **arr){ //Works fine
    for(int row = 0; row<4; row++){
        for(int col = 0; col<4; col++){
            state[row][col] ^= arr[row][col];
        }
    }
}

void subBytesRow(unsigned char *arr, int n){ //Works fine
    int r, c;
    for(int i = 0; i<n; i++){
        r = getFirstBit(arr[i]);
        c = getLastBit(arr[i]);
        arr[i] = sbox[r][c];
    }
}

void invSubBytesRow(unsigned char *arr, int n){ //Works fine
    int r, c;
    for(int i = 0; i<n; i++){
        r = getFirstBit(arr[i]);
        c = getLastBit(arr[i]);
        arr[i] = inv_sbox[r][c];
    }
}

void subBytesState(){ //Works fine
    int r, c;
    for(int i = 0; i<4; i++){
        subBytesRow(state[i], 4);
        // for(int j = 0; j<4; j++){
        //     r = getFirstBit(state[i][j]);
        //     c = getLastBit(state[i][j]);
        //     state[i][j] = sbox[r][c];
        // }
    }
}

void invSubBytesState(){
    int r, c;
    for(int i = 0; i<4; i++){
        invSubBytesRow(state[i], 4);
    }
}

void shiftRow(unsigned char *arr, int n){ //Works fine
    unsigned char temp;
    for(int i = 0; i<n; i++){
        temp = arr[0];
        for(int j = 0; j<3; j++){
            arr[j] = arr[j+1];
        }
        arr[3] = temp;
    }
}

void shiftRows(){ //Works fine
    for(int i = 0; i<4; i++){
        shiftRow(state[i], i);
    }
}

void inverseShiftRows(){
    for(int i = 1; i<4; i++){
        shiftRow(state[4-i], i);
    }
}

unsigned char GMul(unsigned char a, unsigned char b){   //Wikipedia
    unsigned char p = 0x00;

    for(int counter = 0; counter<8; counter++){
        if((b&1)!=0){
            p^=a;
        }
        unsigned char hi_bit_set = (a & 0x80) != 0;
        a <<= 1;
        if(hi_bit_set){
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

void gmix_word(unsigned char *r){   // Works fine
    unsigned char b[4];

    for (int c = 0; c < 4; c++) {
        b[c] = r[c];
    }
    r[0] = GMul(0x02, b[0]) ^ GMul(0x03, b[1]) ^ b[2] ^ b[3];
    r[1] = b[0] ^ GMul(0x02, b[1]) ^ GMul(0x03, b[2]) ^ b[3];
    r[2] = b[0] ^ b[1] ^ GMul(0x02, b[2]) ^ GMul(0x03, b[3]);
    r[3] = GMul(0x03, b[0]) ^ b[1] ^ b[2] ^ GMul(0x02, b[3]);

}

void inverse_gmix_word(unsigned char *r){
    unsigned char b[4];

    for (int c = 0; c < 4; c++) {
        b[c] = r[c];
    }
    r[0] = GMul(0x0e, b[0]) ^ GMul(0x0b, b[1]) ^ GMul(0x0d, b[2]) ^ GMul(0x09, b[3]);
    r[1] = GMul(0x09, b[0]) ^ GMul(0x0e, b[1]) ^ GMul(0x0b, b[2]) ^ GMul(0x0d, b[3]);
    r[2] = GMul(0x0d, b[0]) ^ GMul(0x09, b[1]) ^ GMul(0x0e, b[2]) ^ GMul(0x0b, b[3]);
    r[3] = GMul(0x0b, b[0]) ^ GMul(0x0d, b[1]) ^ GMul(0x09, b[2]) ^ GMul(0x0e, b[3]);

}

void mixColumns(){  // Works fine
    unsigned char word[4];
    for(int col = 0; col<4; col++){
        getWord(state, col, word);
        gmix_word(word);
        for(int row = 0; row<4; row++){
            state[row][col] = word[row];
        }
    }
}

void invMixColumns(){  // Works fine
    unsigned char word[4];
    for(int col = 0; col<4; col++){
        getWord(state, col, word);
        inverse_gmix_word(word);
        for(int row = 0; row<4; row++){
            state[row][col] = word[row];
        }
    }
}

int getKey(){
    printf("key: ");
    scanf("%[^\n]*c", key);
    int len = strlen(key);
    printf("Key length is: %d\n", len);
    switch (mode)
    {
    case 0:
        if(len != 128/8){
            return -1;
        }
        break;
    case 1:
        if(len != 192/8){
            return -1;
        }
        break;
    case 2:
        if(len != 256/8){
            return -1;
        }
        break;
    default:
        return -1;
        break;
    }    
    return 0;
}

void createRoundKeys(){ //Works fine
    roundKeys = (unsigned char ***)malloc(sizeof(unsigned char **) * 11);
    for(int i = 0; i<11; i++){
        roundKeys[i] = (unsigned char **)malloc(sizeof(unsigned char *) * 4);
        for(int j = 0; j<4; j++){
            roundKeys[i][j] = (unsigned char *)malloc(sizeof(unsigned char) * 4);
        }
    }
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            roundKeys[0][i][j] = key[i * 4 + j];
        }
    }
    int pastKey = 0;
    unsigned char word1[4];
    unsigned char word2[4];
    unsigned char wordj[4];
    for(int i = 1; i<11; i++){
        for(int j = 0; j<4; j++){
            if(j - 1 < 0){
                getWord(roundKeys[i-1], (j - 1 + 4), word1);
            } else {
                getWord(roundKeys[i], (j - 1), word1);
            }
            if(j - 4 < 0){
                getWord(roundKeys[i-1], (j - 4 + 4), word2);
            } else {
                getWord(roundKeys[i], j - 4, word2);
            }
            if(j == 0){
                shiftRow(word1, 1);
                subBytesRow(word1, 4);
                for(int idx = 0; idx<4; idx++){
                    if(idx == 0){
                        wordj[idx] = rcon[i - 1] ^ word2[idx] ^ word1[idx];
                    } else {
                        wordj[idx] = 0x00 ^ word2[idx] ^ word1[idx];
                    }
                }
            } else {
                for(int idx = 0; idx < 4; idx ++){
                    wordj[idx] = word1[idx] ^ word2[idx];
                }   
            }
            for(int row = 0; row<4; row++){
                roundKeys[i][row][j] = wordj[row];
            }
        }
    }
}

void initState(){
    state = (unsigned char **)malloc(sizeof(unsigned char *)*4);
    for(int i = 0; i<4; i++){
        state[i] = (unsigned char *)malloc(sizeof(unsigned char)*4);        
    }
}

void disposeState(){
    for(int i = 0; i<4; i++){
        free(state[i]);
    }
    free(state);
}

void disposeRoundKeys(){
    for(int i = 0; i<11; i++){
        for(int j = 0; j<4; j++){
            free(roundKeys[i][j]);
        }
        free(roundKeys[i]);
    }
    free(roundKeys);
}
