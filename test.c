     
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ObjectManager.h"
///////////////////////////////////////////
//CONSTANTS
///////////////////////////////////////////
int testsPassed = 0;
int testsFailed = 0;
//////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////
void testInitPool();
void testDestroyPool();
void testInsert();
void testRetrieveObj();
void testAddReference();
void testDropReference();
void testCompact();
void testDumpPool();

int main(){
///////////////////////////////////////////////////////
//General cases testing
///////////////////////////////////////////////////////
printf("Testing general cases\n");

//////////////////////////////////////////////////////
//General case for initPool
//////////////////////////////////////////////////////
printf("Testing general case for Init pool\n");
testInitPool();
printf("\n/////////////////////////////////////////////////\n");

///////////////////////////////////////////////////////
//Test for destroyPool
//////////////////////////////////////////////////////
printf("\nTesting general destroyPool\n");
testDestroyPool();
printf("\n/////////////////////////////////////////////\n");
///////////////////////////////////////////////////////
//General cases testing for Insertion
///////////////////////////////////////////////////////
printf("Testing general case for Insertion\n");
testInsert();
printf("\n////////////////////////////////////////////\n");

///////////////////////////////////////////////////////
//General cases testing for Retrieve Object
///////////////////////////////////////////////////////
printf("Testing general case for Retrieve Object\n");
testRetrieveObj();
printf("\n////////////////////////////////////////////\n")

///////////////////////////////////////////////////////
//General cases testing for Add reference
///////////////////////////////////////////////////////
printf("Testing general case for  Add reference\n");
testAddReference();
printf("\n//////////////////////////////////////\n");

///////////////////////////////////////////////////////
//General cases testing for drop reference
///////////////////////////////////////////////////////
printf("Testing general case for drop reference\n");
testDropReference();

///////////////////////////////////////////////////////
//General cases testing for Compact
///////////////////////////////////////////////////////
printf("Testing general case for Compact\n");
testCompact();
printf("\n///////////////////////////////////////////////\n");
///////////////////////////////////////////////////////
//General cases testing for dump Pool
///////////////////////////////////////////////////////
printf("Testing general case for dump Pool\n");
testDumpPool();
printf("\n///////////////////////////////////////////////////\n");

///////////////////////////////////////////////////////
//Edge cases testing
///////////////////////////////////////////////////////
printf("Testing edge cases\n");

//////////////////////////////////////////////////////
//General case for edge initPool
//////////////////////////////////////////////////////
printf("Testing edge case for Init pool\n");
testInitPoolEdge();
printf("\n/////////////////////////////////////////////////\n");

///////////////////////////////////////////////////////
//Test for edge destroyPool
//////////////////////////////////////////////////////
printf("\nTesting edge destroyPool\n");
testDestroyPoolEdge();
printf("\n/////////////////////////////////////////////\n");
///////////////////////////////////////////////////////
//General cases testing for Insertion
///////////////////////////////////////////////////////
printf("Testing edge case for Insertion\n");
testInsertEdge();
printf("\n////////////////////////////////////////////\n");

///////////////////////////////////////////////////////
//General cases testing for Retrieve Object
///////////////////////////////////////////////////////
printf("Testing edge case for Retrieve Object\n");
testRetrieveObjEdge();
printf("\n////////////////////////////////////////////\n")

///////////////////////////////////////////////////////
//General cases testing for Add reference
///////////////////////////////////////////////////////
printf("Testing edge case for  Add reference\n");
testAddReferenceEdge();
printf("\n//////////////////////////////////////\n");

///////////////////////////////////////////////////////
//General cases testing for drop reference
///////////////////////////////////////////////////////
printf("Testing edge case for drop reference\n");
testDropReferenceEdge();

///////////////////////////////////////////////////////
//General cases testing for Compact
///////////////////////////////////////////////////////
printf("Testing edge case for Compact\n");
testCompactEdge();
printf("\n///////////////////////////////////////////////\n");
///////////////////////////////////////////////////////
//General cases testing for dump Pool
///////////////////////////////////////////////////////
printf("Testing edge case for dump Pool\n");
testDumpPoolEdge();
printf("\n///////////////////////////////////////////////////\n");

printf("\n/////////////////////////////////////////////////////\n");
printf("Total number of tests executed %d\n", testsPassed+testsFailed);
printf( "\nTotal number of tests passed: %d\n", testsPassed );
printf( "Number of tests failed: %d\n", testsFailed );
}

void testInitPool() {
    initPool();
    if (BlockStart != NULL && BlockEnd == BlockStart && BlockStart->numBytes == 0 && BlockStart->startAddr == 0 && BlockStart->count == 1) {
        printf("Test passed for initPool\n");
        testsPassed++;
    } else {
        printf("Test failed for init pool\n");
        testsFailed++;
    }
}

void testInitPoolEdge() {
    printf("Edge case here is initialising pool twice in a row\n")
    initPool();
    initPool();
    if (BlockStart != NULL && BlockEnd == BlockStart && BlockStart->numBytes == 0 && BlockStart->startAddr == 0 && BlockStart->count == 1) {
        printf("Edge case test passed for init pool\n");
        testsPassed++;
    } else {
        printf("Edge case test failed for init pool\n");
        testsFailed++;
    }
    destroyPool();
}

void testDestroyPool() {
    initPool();
    destroyPool();
    if (BlockStart == NULL && BlockEnd == NULL) {
        printf("Test passe for destroy pool\n");
        testsPassed++;
    } else {
        printf("Test faile for destroy pool\n");
        testsFailed++;
    }
}
void testDestroyPoolEdge() {
    printf("Edge case is destroying pool twice in a row\n")
    initPool();
    destroyPool();
    destroyPool();
    if (BlockStart == NULL && BlockEnd == NULL) {
        printf("Test passed for destroy pool edge case\n");
        testsPassed++;
    } else {
        printf("Test failed for destroy pool edge case\n");
        testsFailed++;
    }
}
void testInsert() {
    initPool();
    Ref ref = insertObject(10);
    if (ref != NULL_REF && BlockStart->next != NULL && BlockStart->next->numBytes == 10 && BlockStart->next->startAddr == 0 && BlockStart->next->count == 1) {
        printf("Test passed for insertion\n");
        testsPassed++;
    } else {
        printf("Test failed for insertion\n");
        testsFailed++;
    }
}
void testInsertEdge() {
    printf("Edge case here is inserting an object larger than the pool size\n");
    initPool();
    Ref ref = insertObject(MEMORY_SIZE + 1);
    if (ref == NULL_REF) {
        printf("Test passed for insertion edge case\n");
        testsPassed++;
    } else {
        printf("Test failed for insertion edge case\n");
        testsFailed++;
    }
}

void testRetrieveObj() {
    initPool();
    Ref ref = insertObject(10);
    void *obj = retrieveObject(ref);
    if (obj != NULL) {
        printf("Test passed for retrieve Object\n");
        testsPassed++;
    } else {
        printf("Test failed for retrieve Object\n");
        testsFailed++;
    }
}
void testRetrieveObjEdge() {
    printf("Edge case here is retrieving a non-existent object\n");
    initPool();
    void *obj = retrieveObject(NULL_REF);
    if (obj == NULL) {
        printf("Test passed for retrieve Object edge case\n");
        testsPassed++;
    } else {
        printf("Test failed for retrieve Object edge case\n");
        testsFailed++;
    }
}
void testAddReference() {
    initPool();
    Ref ref = insertObject(10);
    addReference(ref);
    Block *tmp = BlockStart->next;
    if (tmp->count == 2) {
        printf("Test passed for add reference\n");
        testsPassed++;
    } else {
        printf("Test failed for add reference\n");
        testsFailed++;
    }
}
void testAddReferenceEdge() {
    printf("Edge case here is adding a reference to a non-existent object\n");
    initPool();
    addReference(NULL_REF);
    printf("Block with given reference is not found\n");
    printf("Test passed: Add reference edge case\n");
    testsPassed++;
}
void testDropReference() {
    initPool();
    Ref ref = insertObject(10);
    addReference(ref);
    dropReference(ref);
    Block *tmp = BlockStart->next;
    if (tmp->count == 1) {
        printf("Test passed for drop reference\n");
        testsPassed++;
    } else {
        printf("Test failed for drop reference\n");
        testsFailed++;
    }
}
void testDropReferenceEdge() {
    printf("Edge case here is dropping a reference to a non-existent object\n");
    initPool();
    dropReference(NULL_REF);
    printf("Block with given reference is not found\n");
    printf("Test passed for drop reference edge case\n");
    testsPassed++;
}
void testCompact() {
    initPool();
    Ref ref1 = insertObject(10);
    Ref ref2 = insertObject(10);
    dropReference(ref1);
    compact();
    Block *tmp = BlockStart->next;
    if (tmp->numBytes == 10 && tmp->startAddr == 0) {
        printf("Test passed for compact\n");
        testsPassed++;
    } else {
        printf("Test failed for compact\n");
        testsFailed++;
    }
}
void testCompactEdge() {
    printf("Edge case here is compacting an empty pool\n");
    initPool();
    compact();
    if (BlockStart != NULL && BlockEnd == BlockStart && BlockStart->numBytes == 0 && BlockStart->startAddr == 0 && BlockStart->count == 1) {
        printf("Test passed for compact edge case\n");
        testsPassed++;
    } else {
        printf("Test failed for compact edge case\n");
        testsFailed++;
    }
}
void testDumpPool() {
    initPool();
    Ref ref = insertObject(10);
    addReference(ref);
    addReference(ref);
    dumpPool();
    printf("Test passed: Dump pool\n");
    testsPassed++;
}

void testDumpPoolEdge() {
    initPool();
    dumpPool();
    printf("Test passed for dump pool edge case\n");
    testsPassed++;
}