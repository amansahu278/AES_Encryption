#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

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


void disposeRoundKeys(){
    for(int i = 0; i<11; i++){
        for(int j = 0; j<4; j++){
            free(roundKeys[i][j]);
        }
        free(roundKeys[i]);
    }
    free(roundKeys);
}
