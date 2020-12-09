#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

int getNumRounds(){
    switch (mode)
    {
    case 0: return 11;
        break;
    case 1: return 13;
    case 2: return 15;
    default:
        return -1;
        break;
    }
}

int getKeyLen(){
    switch (mode)
    {
    case 0:
        return 128/8;
        break;
    case 1:
        return 192/8;
        break;
    case 2: return 256/8;
        break;
    default:
        return -1;
        break;
    }
}

int getKey(){
    printf("key: ");
    scanf("%[^\n]*c", key);
    int len = strlen(key);
    printf("Key length is: %d\n", len);
    
    if(len != getKeyLen()){
        return -1;
    }
    return 0;
}

void createRoundKeys(){ //Works fine
    /*
        128 bit:
            11 total keys
            each key has 4 words
            1 word is 4 bytes
        192 bit:
            13 total keys
            192/8 = 24 bytes = 24/4 = 6 words
            each word has 4 bytes
        256 bit:
            15 total keys
            256/8 = 32 bytes = 32/4 = 8 words
            each word has 4 bytes
    */
   int totalKeys = getNumRounds();
   int numWords = getKeyLen()/4;
   int wordLength = 4;

    roundKeys = (unsigned char ***)malloc(sizeof(unsigned char **) * totalKeys);
    for(int i = 0; i<totalKeys; i++){
        roundKeys[i] = (unsigned char **)malloc(sizeof(unsigned char *) * numWords);
        for(int j = 0; j<4; j++){
            roundKeys[i][j] = (unsigned char *)malloc(sizeof(unsigned char) * wordLength);
        }
    }
    for(int i = 0; i<numWords; i++){
        for(int j = 0; j<wordLength; j++){
            roundKeys[0][j][i] = key[i * wordLength + j];
        }
    }

    int pastKey = 0;
    unsigned char word1[4];
    unsigned char word2[4];
    unsigned char wordj[4];
    
    for(int i = 1; i<totalKeys; i++){
        for(int j = 0; j<numWords; j++){
            if(j - 1 < 0){
                getWord(roundKeys[i-1], (j - 1 + numWords), word1);
            } else {
                getWord(roundKeys[i], (j - 1), word1);
            }
            if(j - numWords < 0){
                getWord(roundKeys[i-1], (j - numWords + numWords), word2);
            } else {
                getWord(roundKeys[i], j - numWords, word2);
            }
            if(j == 0){
                shiftRow(word1, 1);
                subBytesRow(word1, 4);
                for(int idx = 0; idx<4; idx++){
                    if(idx == 0){
                        wordj[idx] = rcon[i] ^ word2[idx] ^ word1[idx];
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
    int totalKeys = getNumRounds();
    for(int i = 0; i<totalKeys; i++){
        for(int j = 0; j<4; j++){
            free(roundKeys[i][j]);
        }
        free(roundKeys[i]);
    }
    free(roundKeys);
}
