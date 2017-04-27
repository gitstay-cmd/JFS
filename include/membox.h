#ifndef membox_h_INCLUDED
#define membox_h_INCLUDED
#include <block.h>

struct membox{
    struct block_t block;
};

struct membox *init();
#endif

