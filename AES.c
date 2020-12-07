/*
    128 - bit AES  
    Main steps:
    a. Encryption
    b. Key schedule

    a. Encryption:
        Overall:
        State = X
        1. AddRoundKey(State, Key0)
        2. for r = 1 to (Nr - 1)
            a. SubBytes(State, S-box)
            b. ShiftRows(State)
            c. MixColumns(State)
            d. AddRoundKey(State, Keyr)
        end for
        1. SubBytes(State, S-box)
        2. ShiftRows(State)
        3. AddRoundKey(State, KeyNr)
        Y = State

    b. Key schedule:
        For 128 - bit AES there will be a total of 11 keys (10 round keys + 1 original key)
        (Imagine all keys are lined up next to each other)

        We repeat this for 10 times, K1 -> K10
        For each Wi out of the 4 words in a key:
            if i % 4 == 0:
                selected = w[i-1]
                selected = lshift the selected word by 1
                selecte = subbyte the word
                w[i] =  selected ^ w[i-4] ^ rcon[i] (which is (rcon[i], 0, 0, 0)T)
            else:
                wi = w[i-1] ^ w[i - 4]

    TODO: 
    1. Expansion of key
    2. SubBytes
    3. ShiftRows
    4. MixColumns

    Things to be copied directly;
    1. sbox
    2. rcon
    3. Math function for mix columns as it is too complicated/long to implement by self
        - Probably could be done given it wasn't 2:45 am and i wasnt so sleepy

    About the functions: (Pun intended)
    1. AddRoundKey
        - XOR of round key to the state
    
    2. SubBytes
        - Substitute the words using the sbox
    
    3. Shift rows
        - LShift the ith row by i, i={0,1,2,3} 
    
    4. MixColumns 
        
        - The matrix of a particular Galois Field.
        Example: 3x^3 + x^2 + x + 2 would have the matrix
        [
            2, 3, 1, 1,
            1, 2, 3, 1,
            1, 1, 2, 3,
            3, 1, 1, 2
        ]
        if a word of the state is (b1, b2, b3, b4)T
        then the mixColumns operation would produce
        b1 = (b1 X 2) ^ (b2 X 3) ^ (b3 X 1) ^ (b4 X 1)
        similarly
        b2 = (b1 X 1) ^ (b2 X 2) ^ (b3 X 3) ^ (b4 X 2)
        etc
        The above multiplication is performed over a Galois Field.
        For each term, we perform lookups on various tables. So that it is appropriately modulo'd.
        Or instead, copy the function from the internet which does the same.

    Important points to remember:
    - Every word of the State or the Key are taken column wise
    - Similarly, operations are also done in the same orientation

    References: 
        - Class notes / ppt
        - https://www.lri.fr/~fmartignon/documenti/systemesecurite/5-AES.pdf 
        - https://www.comparitech.com/blog/information-security/what-is-aes-encryption/#How_does_AES_work
        - https://www.youtube.com/watch?v=gP4PqVGudtg

        MixColumns
        - http://www.angelfire.com/biz7/atleast/mix_columns.pdf
        - https://en.wikipedia.org/wiki/Rijndael_MixColumns
        - http://www.infosecwriters.com/text_resources/pdf/AESbyExample.pdf

    - Author:
        Aman Sahu

    --- Procedure Followed:
        - Made all the side functions and tested their working
        - Implemented the key expansion algorithm
            * Checked the working by taking the same key as in video mentioned above and manually checking if same results are recreated
        - Implemented the encryption algorithm
        - Problems encountered, undesired output
            * Print the state for every step showed that my initial fault was that i was doing a simple multiplication for mixColumns, 
              i did not take into consideration the restrictions of the Field.
            * Played around, trying to implement it.
            * Googled around, turns out it is a rather difficult calculation, so i decided to borrow the function from wikipedia, 
              an alternative would have been to copy the lookup tables, but the former seems more dynamic
        - After that, i was able to recreate the same outputs and intermediates as per the video for that key and plain text
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getFirstBit(unsigned char);
int getLastBit(unsigned char);
void addRoundKey(unsigned char **);
void subBytesRow(unsigned char *arr, int n);
void subBytesState();
void shiftRows();
void inverseShiftRows();
void shiftRow(unsigned char *, int);
void gmix_column(unsigned char *);
void mixColumns();
void getWord(unsigned char **, int , unsigned char *);
void createRoundKeys();
int encryptPt(unsigned char [], unsigned char []);
void initState();
void disposeState();
void disposeRoundKeys();

unsigned char **state;
unsigned char key[] = {
        0x2b, 0x28, 0xab, 0x09,
        0x7e, 0xae, 0xf7, 0xcf,
        0x15, 0xd2, 0x15, 0x4f,
        0x16, 0xa6, 0x88, 0x3c
    };
// unsigned char key[] = {
//         'k', ' ', ' ', 'i',
//         'e', 'a', 'b', 'n',
//         'y', 'r', 'o', 'g',
//         's', 'e', 'r', 0x01
//     };
unsigned char ***roundKeys;

unsigned char mixField[4][4] = { //UNUSED
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};

static const unsigned char sbox[16][16] = {
        //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  /*0*/{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
  /*1*/{0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
  /*2*/{0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
  /*3*/{0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
  /*4*/{0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
  /*5*/{0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
  /*6*/{0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
  /*7*/{0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
  /*8*/{0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
  /*9*/{0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
  /*A*/{0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
  /*B*/{0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
  /*C*/{0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
  /*D*/{0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
  /*E*/{0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
  /*F*/{0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

static const unsigned char inv_sbox[16][16] = {
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
    {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
    {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
    {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
    {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
    {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
    {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
    {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
    {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
};

static const unsigned char rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

int getFirstBit(unsigned char ch){
    return ch >> 4;
}

int getLastBit(unsigned char ch){
    return ch & 0x0F;
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

void gmix_column(unsigned char *r) { // Works fine
    // Copied from the internet
    unsigned char a[4];
    unsigned char b[4];
    unsigned char c;
    unsigned char h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
    for (c = 0; c < 4; c++) {
        a[c] = r[c];
        /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (unsigned char)((signed char)r[c] >> 7); /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = r[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= 0x1B & h; /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

void mixColumns(){  // Works fine
    unsigned char word[4];
    for(int col = 0; col<4; col++){
        getWord(state, col, word);
        gmix_column(word);
        // gmix_word(word);
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

void getWord(unsigned char **arr, int n, unsigned char *val){ //Works fine
    for(int i = 0; i<4; i++){
        val[i] = arr[i][n];
    }
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
                if(currLen > strlen(PT)){
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

int main(){

    initState();
    createRoundKeys();
    unsigned char PT[112];
    // unsigned char PT[112] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    printf("Enter the plain text: ");
    fgets(PT, 100, stdin);
    PT[strlen(PT)-1] = '\0';
    unsigned char CT[112];
    unsigned char DT[112];
    
    printf("PT is: ");
    for(int i = 0; i< strlen(PT); i++){
        printf("%x ", PT[i]);
    }
    printf("\n");
    printf("\n");

    int n = encryptPt(PT, CT);
    
    printf("CT length is: %d\n", n*16);
    printf("CT is: %s\n", CT);
    for(int i = 0; i<n*16 ; i++){
        printf("%x ", CT[i]);
    }
    printf("\n");
printf("\n");
    decryptPt(CT, DT, n);
    
    printf("DT length is: %ld\n", strlen(DT));
    printf("DT is: %s\n", DT);
    for(int i = 0; i<n*16 ; i++){
        printf("%x ", DT[i]);
    }
    printf("\n");
    printf("\n");
    disposeState();
    disposeRoundKeys();
    return 0;
}
