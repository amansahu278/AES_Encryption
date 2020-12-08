#include<stdio.h>
#include<string.h>
#include "operations.h"
#include "constants.h"

int check(unsigned char CT[16]){
    for(int i = 0; i<16; i++){
        if(CT[i] == 0x80){
            if(i == 15 || CT[i+1] == 0x00){
                return i;
            }
        } 
    }
    return 16;
}

int decryptPt(char CT[]){ //Works fine
    int len, keyIdx, n;
    int totalKeys = getNumRounds();
    int ret = 0;

    unsigned char toWrite[16];
    char destName[100] ;

    FILE *in = fopen(CT, "r");
    if(in == NULL){
        printf("couldn't open file.\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_END);
    n = ftell(in)/16;
    fseek(in, 0, SEEK_SET);

    strncpy(destName, CT, strlen(CT)-4);
    destName[strlen(CT)-4] = '\0';
    printf("Decrypted name: %s", destName);
    FILE *out = fopen(destName, "w");


    for(int i = 0; i<n; i++){
        keyIdx = totalKeys-1;

        ret = fread((unsigned char *)toWrite, 1, 16, in);
        if(ret < 16){
            printf("Not sufficient characters\n");
            return -1;
        }
        for(int c = 0; c<4; c++){
            for(int r = 0; r<4; r++){    
                state[r][c] = toWrite[c*4 + r];
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
                toWrite[c*4 + r] = state[r][c];
            }
        }
        fwrite((void *)toWrite, 1, 16, out);
    }
    fclose(out);
    fclose(in);
    in = fopen(destName, "r");
    if(in == NULL){
        printf("couldn't open file.\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_END);
    len = ftell(in);
    printf("Length of the decrypted file was: %d", len);
    fseek(in, 0, SEEK_SET);
    fclose(in);
    return n;
}
