#define B_LOCK_SIZE 512
#define MAX_BLOCKAGE 60
#include <stdlib.h>
#include <stdint.h>

uint8_t *memory;

void memory_init(){
    memory = (uint8_t *)calloc(B_LOCK_SIZE * MAX_BLOCKAGE, sizeof(uint8_t));
 
}
