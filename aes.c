#include <stdio.h>
#include "aes.h"

uint8_t targeted_round = 10;

int main(){
    uint8_t ciphertext [DATA_SIZE];
    uint8_t plaintext [DATA_SIZE] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff}; 
    uint8_t key [DATA_SIZE] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

    printf("PLAINTEXT: ");
    for(int i=0; i<DATA_SIZE; i++){
        printf("%x ", plaintext[i]);
    }

    printf("\nKEY: ");
    for(int i=0; i<DATA_SIZE; i++){
        printf("%x ", key[i]);
    }

    printf("\n");
    AESEncrypt(ciphertext, plaintext, key);

    return 0;
}