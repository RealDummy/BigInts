
/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa7
 */

#include "List.h"
#include <stdio.h>
#include <assert.h>


//Im only going to test the functions that I changed, as I am pretty sure this already works
int main(){

    assert(sizeof(LIST_ELEMENT) == 8);

    List A = newList();

    append(A,10000000000000);
    prepend(A,-999999999999999);
    moveFront(A);
    moveNext(A);
    insertBefore(A,888888888888);
    insertAfter(A, 7777777777777);
    for(moveFront(A); index(A) != LIST_CURSOR_UNDEFINED; moveNext(A)){
        LIST_ELEMENT val = get(A);
        printf("%"LIST_FORMAT"\n",val);
    }

    freeList(&A);
    return 0;
}