#include<stdio.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

int decryptPt(unsigned char CT[], unsigned char DT[], int n){ //Works fine
    int len = strlen(CT);
    int currLen = 0;
    int writeLen = 0;
    // int n = len/16;
    int keyIdx;

    for(int i = 0; i<n; i++){
        keyIdx = 10;
        for(int c = 0; c<4; c++){
            for(int r = 0; r<4; r++){    
                state[r][c] = CT[currLen ++];
            }
        }
        
        addRoundKey(roundKeys[keyIdx --]);
        inverseShiftRows();
        invSubBytesState();

        for(; keyIdx >= 1; keyIdx --){
            addRoundKey(roundKeys[keyIdx]);
            invMixColumns();
            inverseShiftRows();
            invSubBytesState();
        }
        
        addRoundKey(roundKeys[keyIdx]);
        for(int c = 0; c<4; c++){
            for(int r = 0; r<4; r++){
                DT[writeLen ++] = state[r][c];
            }
        }
    }
    return n;
}
