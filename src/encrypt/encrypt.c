#include<stdio.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

int encryptPt(unsigned char PT[], unsigned char CT[]){ //Works fine
    int len = strlen(PT);
    int currLen = 0;
    int writeLen = 0;
    int n = len/16;
    int keyIdx = 0;
    int end = 0;
    
    if( 16 * n < len ){
        n++;
    }
    
    for(int i = 0; i<n; i++){
        keyIdx = 0;
        for(int c = 0; c<4; c++){
            for(int r = 0; r<4; r++){
                if(currLen >= strlen(PT)){
                    if(!end){
                        end = !end;
                        state[r][c] = 0x80;
                    } else {
                        state[r][c] = 0x00;
                    }
                    currLen ++;
                } else {
                    state[r][c] = PT[currLen ++];
                }
            }
        }

        addRoundKey(roundKeys[keyIdx ++]);

        for(; keyIdx < 10; keyIdx ++){
            subBytesState();
            shiftRows();
            mixColumns();
            addRoundKey(roundKeys[keyIdx]);
        }
     
        subBytesState();
        shiftRows();
        addRoundKey(roundKeys[keyIdx]);
     
        for(int c = 0; c<4; c++){
            for(int r = 0; r<4; r++){
                CT[writeLen ++] = state[r][c];
            }
        }
    }
    return n;
}