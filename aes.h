#ifndef AES_H
#define AES_H
#define DATA_SIZE 16
#define STATE_ROW_SIZE 4
#define STATE_COL_SIZE 4
#define ROUND_COUNT 10
#include <stdint.h>
#include <stdio.h>
#include "constants.h"

// the round that will trigger
extern uint8_t targeted_round;
void AESEncrypt (uint8_t ciphertext [ DATA_SIZE ], uint8_t plaintext [ DATA_SIZE ] , uint8_t key [ DATA_SIZE ]);

void AddRoundKey (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t roundkey [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);
void SubBytes (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);
void ShiftRows (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);
void MixColumns (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);
uint8_t GF(uint8_t a, uint8_t b);

void KeyGen (uint8_t roundkeys [][ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t master_key [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);

// fill the first column of a given round key
void ColumnFill (uint8_t roundkeys [][ STATE_ROW_SIZE ][ STATE_COL_SIZE ] , int round);

// fill the other 3 columns of a given round key
void OtherColumnsFill (uint8_t roundkeys [][ STATE_ROW_SIZE ][ STATE_COL_SIZE ], int round);
void GetRoundKey (uint8_t roundkey [ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t roundkeys [][ STATE_ROW_SIZE ][ STATE_COL_SIZE ], int round);
void MessageToState (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t message [ DATA_SIZE ]);
void StateToMessage (uint8_t message [ DATA_SIZE ], uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ]);
void MCMatrixColumnProduct (uint8_t colonne [ STATE_COL_SIZE ]);

uint8_t gmul (uint8_t a, uint8_t b);
extern const uint8_t rcon [10];
extern const uint8_t sboxtab [256];
extern const uint8_t invsbox [256];

extern const uint8_t pred_matrix [DATA_SIZE];

void AESEncrypt (uint8_t ciphertext [ DATA_SIZE ], uint8_t plaintext [ DATA_SIZE ] , uint8_t key [ DATA_SIZE ])
{
    ciphertext = plaintext;
    uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ];
    uint8_t roundkey [ STATE_ROW_SIZE ][ STATE_COL_SIZE ];
    uint8_t master_key [ STATE_ROW_SIZE ][ STATE_COL_SIZE ];
    int flag = 0;

    printf("\ninput: ");

    for(int i=0; i<STATE_ROW_SIZE; i++){
        for(int j=0; j<STATE_COL_SIZE; j++){
            state[i][j] = ciphertext[flag];
            roundkey[i][j] = key[flag];
            master_key[i][j] = key[flag];

            flag++;
            printf("%x ", state[i][j]);
        }
    }

    AddRoundKey(state, roundkey);

    for(int i=0; i<targeted_round; i++){
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        KeyGen(roundkey, master_key);
        AddRoundKey(state, roundkey);
    }

    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);

    printf("\noutput: ");

    for(int i=0; i<STATE_ROW_SIZE; i++){
        for(int j=0; j<STATE_COL_SIZE; j++){
            printf("%x ", state[i][j]);
        }
    }
}

void AddRoundKey (uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t roundkey [ STATE_ROW_SIZE ][ STATE_COL_SIZE ])
{
    printf("\nstart: ");

    for(int i=0; i<STATE_ROW_SIZE; i++){
        for(int j=0; j<STATE_COL_SIZE; j++){
            state[i][j] = state[i][j] ^ roundkey[i][j];
            printf("%x ", state[i][j]);
        }
    }    
}

void SubBytes(uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ])
{
    printf("\ns_box: ");

    for(int i=0; i<STATE_ROW_SIZE; i++){
        for(int j=0; j<STATE_COL_SIZE; j++){
            state[i][j] = sboxtab[state[i][j]];
            printf("%x ", state[i][j]);
        }
    }   
}

void ShiftRows(uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ])
{  
    uint8_t tmp = 0;      

    for(int i=1; i<STATE_ROW_SIZE; i++){
        for (int j=0; j<i; j++)
        {
            tmp = state[0][i];
            for (int k = 0; k < 3; k++){
                state[k][i] = state[k+1][i];
            }
            state[3][i] = tmp;
        }
    }  

    printf("\ns_row: ");

    for(int i=0; i<STATE_ROW_SIZE; i++){
        for(int j=0; j<STATE_COL_SIZE; j++){
            printf("%x ", state[i][j]);
        }
    }   
}

void MixColumns(uint8_t state [ STATE_ROW_SIZE ][ STATE_COL_SIZE ])
{
    printf("\nm_col: ");

    int flag = 0;
    uint8_t results[4];
    uint8_t col[4];

    for(int i=0; i<STATE_COL_SIZE; i++){

        for(int k=0; k<4; k++){
            col[k] = state[i][k];
        }

        flag = 0;

        for(int j=0; j<STATE_ROW_SIZE; j++){

            for(int k=0; k<4; k++){
                results[k] = GF(col[k], pred_matrix[flag]);
                flag++;
            }

            state[i][j] = results[0] ^ results[1] ^ results[2] ^ results[3];
            
            printf("%x ", state[i][j]);
        }
    }  
}

uint8_t GF(uint8_t a, uint8_t b)
{
    uint8_t p = 0;
    uint8_t hi_bit_set;
    for (int counter=0; counter<8; counter++)
    {
        if ((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void KeyGen (uint8_t roundkey [ STATE_ROW_SIZE ][ STATE_COL_SIZE ], uint8_t master_key [ STATE_ROW_SIZE ][ STATE_COL_SIZE ])
{
    //Por hacer

}

#endif