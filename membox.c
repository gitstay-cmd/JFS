#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <membox.h>
extern uint8_t *memory;

#define BLOCK_SIZE_T 512
extern void memory_init();

int membox_write(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t sector, uint32_t block_num);

int membox_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t sector, uint32_t block_num);

struct membox *init(){
	memory_init();
	struct membox *mem = (struct membox *)malloc(sizeof(struct membox));
	mem->block.block_size = BLOCK_SIZE_T;
	mem->block.read = membox_read;
	mem->block.write = membox_write;
	return mem;
}

int membox_write(struct block_t *dev, uint8_t *buf, size_t buf_size,uint32_t sector, uint32_t block_num){

	uint32_t start_address = (uint32_t)(BLOCK_SIZE_T * block_num);
	uint32_t end_address = start_address + buf_size;
	
	if(buf_size > end_address - start_address){
    	buf_size = (end_address - start_address);
	}
	int i =0;
	for(i = 0; i < buf_size-1; i++){
    	memory[start_address + sector + i] = buf[i];
	}
	buf[i] = '\0'; 
    //memcpy((void*)(memory + start_address + sector), buf, buf_size);

    return (int)buf_size;
    
}

int membox_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t sector, uint32_t block_num){
	uint32_t start_address = (uint32_t)(BLOCK_SIZE_T * block_num);
	uint32_t end_address = start_address + buf_size;

	if(buf_size > end_address - start_address){
    	buf_size = (end_address - start_address);
	}

	for(int i = 0; i < buf_size; i++){
		buf[i] = memory[start_address + sector + i];
	}

	//memcpy(buf, (void*)(memory +start_address), buf_size);

	return (int)buf_size;
}

