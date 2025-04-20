
#include "ObjectManager.h"
#include <stdlib.h>
#include <stdio.h>
static Ref ref = 1;

//////////////////////////////////////////
//Struct Block containing information on it
//numBytes- size
//startAddr-where object starts
//ref- reference used to identify object
//count-number of references to this object
//next- pointer to next block
///////////////////////////////////////////
typedef struct BLOCK {
    int numBytes;    
    int startAddr;  
    Ref ref;         
    int count;    
    Block *next;
}Block;

///////////////////////////////////////////////////////////////////////////////////////
// The blocks are stored in a linked list where the start of the list is pointed to by memBlockStart.
// BlockStart-start of linked list of blocks allocated
// end of linked list of blocks allocated
////////////////////////////////////////////////////////////////////////////////////////
static Block *BlockStart; 
static Block *BlockEnd;  

/////////////////////////////////////////////////////////////
//Two buffers used during compact(), garbage collection
//currBuffer- pointer to the buffer we're currently using
////////////////////////////////////////////////////////////
static unsigned char buffer1[MEMORY_SIZE];
static unsigned char buffer2[MEMORY_SIZE];
static unsigned char *currBuffer = buffer1;
void compact();
///////////////////////////////////////////////
//Initialize object manager 
//////////////////////////////////////////////
void initPool() {
    BlockStart = (Block *) malloc(sizeof(Block));
    BlockStart->next = NULL;
    BlockStart->numBytes = 0;
    BlockStart->startAddr = 0;
    BlockStart->count = 1;
    BlockEnd = BlockStart;
}
///////////////////////////////////////
//Cleans up object manager
////////////////////////////////////////
void destroyPool() {
    Block *tmp = BlockStart;
    while(tmp != NULL) {
        Block *tmpToDelete = tmp;
        tmp = tmp->next;
        free(tmpToDelete);
    }
    BlockStart = NULL;
BlockEnd = NULL;
}

// Adds the given object into our buffer. It will fire the garbage collector as required.
// On success it returns the reference number for the block allocated.
// On failure it returns NULL_REF (0)
Ref insertObject(ulong size) {
    if (size + BlockEnd->startAddr + BlockEnd->numBytes > MEMORY_SIZE) {
        compact();
        if(size + BlockEnd->startAddr + BlockEnd->numBytes > MEMORY_SIZE){
return NULL_REF;
        }
    }
    Block* newBlock= (Block *) malloc(sizeof(Block));
    newBlock->numBytes = size;
    newBlock->startAddr = BlockEnd->startAddr + BlockEnd->numBytes;
    newBlock->count = 1;
    newBlock->ref = ref;
    newBlock->next = NULL;
    ref += 1;
    BlockEnd->next=newBlock;
    BlockEnd=newBlock;
    return ref - 1;
}

////////////////////////////////////////////////
// Retrieves the address of an object by reference id
///////////////////////////////////////////////////
void *retrieveObject( Ref ref) {
    Block *tmp = BlockStart->next;
    while (tmp != NULL) {
        if (tmp->ref == ref) {
            return &currBuffer[tmp->startAddr];
        }
        tmp = tmp->next;
    }
    return NULL;
}

////////////////////////////////////////////////////////////////
//Increment the reference count for the object with reference id
/////////////////////////////////////////////////////////////
void addReference( Ref ref) {
    Block *tmp = BlockStart->next;
    while (tmp != NULL) {
        if (tmp->ref == ref) {
            tmp->count ++;
            return;
        }
        tmp = tmp->next;
    }
printf("Block with given reference is not found\n");
}


///////////////////////////////////////////////////////// 
// update our index to indicate that a reference is gone
///////////////////////////////////////////////////////////
void dropReference(const Ref ref) {
    Block *tmp = BlockStart->next;
    while (tmp != NULL) {
        if (tmp->ref == ref) {
               tmp->count--;
               if(tmp->count==0){
                return;
               }
        }
        tmp = tmp->next;
    }
}

/////////////////////////////////////////
// performs our garbage collection
/////////////////////////////////////////
void compact() {
  Block *tmpCurr = BlockStart->next;
    Block* tmpPrev=BlockStart;
    // Set the current buffer as active and the other buffer as inactive
    unsigned char *activeBuffer = currBuffer;
    unsigned char *inactiveBuffer;

    if (currBuffer == buffer1) {
        inactiveBuffer = buffer2;
    } else {
        inactiveBuffer = buffer1;
    }
int newStart=0;
    while (tmpCurr != NULL) {
        if (tmpCurr->count > 0) {
            for (int i = 0; i < tmpCurr->numBytes; i++) {
                inactiveBuffer[newStart + i] = activeBuffer[tmpCurr->startAddr + i];
            }
             tmpCurr->startAddr = newStart;
            newStart += tmpCurr->numBytes;
            tmpPrev=tmpCurr;
            tmpCurr=tmpCurr->next;
        }
else{
Block *toDelete = tmpCurr;
tmpPrev->next = tmpCurr->next;
tmpCurr=tmpPrev->next;
free(toDelete);
}
    }
    BlockEnd=tmpPrev;

    // Switch buffers: the current buffer becomes inactive and vice versa
    if (currBuffer == buffer1) {
        currBuffer = buffer2;
    } else {
        currBuffer = buffer1;
    }
}

////////////////////////////////////////////
//Print (to stdout) info about each object
// that is currently allocated including itsid, start address, and size.
/////////////////////////////////////////////
void dumpPool() {
    Block *tmp = BlockStart->next;
    int blockCount=1;
    while (tmp != NULL) {
        if (tmp->count>=0) {
            printf("Block number:%d, Start:%d, Size:%d, Reference count:%d, Reference:%lu\n",blockCount, tmp->startAddr, tmp->numBytes, tmp->count, tmp->ref );
        }
         blockCount++;
        tmp = tmp->next;
    }
}
