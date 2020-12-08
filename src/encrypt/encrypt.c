#include<stdio.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

int encryptPt(char path[]){ //Works fine
    int len, n, ret, k, totalKeys, end = 0, keyIdx;
    FILE *in , *out;
    
    in = fopen(path, "r");
    if(in == NULL){
        printf("couldn't open file.\n");
        return -1;
    }
    fseek(in, 0, SEEK_END);
    len = ftell(in);
    n = len/16;
    fseek(in, 0, SEEK_SET);

    totalKeys = getNumRounds();
    keyIdx = 0;
    end = 0;
    
    if( 16 * n < len ){
        n++;
    }
    
    unsigned char toWrite[16];
    char destPath[100];
    sprintf(destPath, "%s_enc", path);
    
    out = fopen(destPath, "w");

    for(int i = 0; i<n; i++){
        keyIdx = 0;
        ret = fread((unsigned char *)toWrite, 1, 16, in);
        for(k = 0; k<ret; k++){
            state[k%4][k/4] = toWrite[k];
        }
        while(k<16){
            if(!end){
                end = !end;
                state[k%4][k/4] = 0x80;
            } else {
                state[k%4][k/4] = 0x00;
            }
            k++;
        }

        addRoundKey(roundKeys[keyIdx ++]);

        for(; keyIdx < totalKeys-1; keyIdx ++){
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
                toWrite[c*4 + r] = state[r][c];
            }
        }
        
        fwrite((void *)toWrite, 1, 16, out);
        
    }
    fclose(in);
    fclose(out);
    return n;
}