/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa3
 */
#ifndef PA7_LIST_H
#define PA7_LIST_H
#include <stdio.h>
#include <inttypes.h>
//some function explanations from pa1.pdf
#define LIST_CURSOR_UNDEFINED -1

#define LIST_ELEMENT int64_t

//don't include the % sign
#define LIST_FORMAT PRId64

typedef struct ListObj* List;

//construct / destruct-------------

// returns an empty List
List newList();

// frees List and sets it's ptr to NULL
void freeList(List* pL);

//Access---------------------------

// Returns the number of elements in L.
int length(List L); 

// Returns index of cursor element if defined, CURSOR_UNDEFINED otherwise.
int index(List L); 

// Returns front element of L. 
// Pre: length()>0
LIST_ELEMENT front(List L); 

// Returns back element of L. 
// Pre: length()>0
LIST_ELEMENT back(List L); 

// Returns cursor element of L. 
// Pre: length()>0, index()>=0
LIST_ELEMENT get(List L); 

// Returns true (1) iff Lists A and B are in same state, and returns false (0) otherwise.
int list_equals(List A, List B); 


//Manipulation----------------------

// Resets L to its original empty state.
void clear(List L); 

// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, LIST_ELEMENT x); 


// If L is non-empty, sets cursor under the front element,otherwise does nothing.
void moveFront(List L); 

// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L); 

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined do nothing
void movePrev(List L); 

// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined do nothing
void moveNext(List L); 

// Adds x to the front of L
void prepend(List L, LIST_ELEMENT x); 

// Adds x to the end of L
void append(List L, LIST_ELEMENT x); 

// inserts x before L's Cursor, err if cursor is undefined
void insertBefore(List L, LIST_ELEMENT x); 

// inserts x after L's Cursor, err if cursor is undefined (increases cursor index)
void insertAfter(List L, LIST_ELEMENT x); 

// Delete the front element. 
//Pre: length()>0
void deleteFront(List L); 

// Delete the back element. 
//Pre: length()>0
void deleteBack(List L); 

// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L); 

//utility---------------------------

// Prints the list out on one line with front on the left and end on the right.
// Prints to out
void printList(FILE* out, List L);

// Copies L and returns a pointer to the copy.
// Cursor index is not copied, and 
List copyList(List L);




#endif //PA7_LIST_H
