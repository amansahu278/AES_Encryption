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

int islegalPath(char path[]){
    char needle[] = "_enc";
    return strstr(path, needle) != NULL;
}

int decryptPt(char path[]){ //Works fine
    int len, keyIdx, n;
    int totalKeys = getNumRounds();
    int ret = 0;

    unsigned char toWrite[16];
    char destName[100] ;

    if(!islegalPath(path)){
        printf("Cannot decrypt such file. The encrypted file should end with _enc\n");
        return -1;
    }

    FILE *in = fopen(path, "r");
    if(in == NULL){
        printf("Couldn't open file.\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_END);
    n = ftell(in)/16;
    fseek(in, 0, SEEK_SET);

    strncpy(destName, path, strlen(path)-4);
    destName[strlen(path)-4] = '\0';
    printf("Decrypted filename: %s\n", destName);
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
    return n;
}
