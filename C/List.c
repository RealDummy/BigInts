/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa3
 */
#include "List.h"

#include <stdlib.h>


#define error(op,pre) fprintf(stderr,"List Error: Calling %s on %s\n",op,pre); exit(1);
#define nullGuard(list,op) if(!(list)) {error(op,"null ptr");}


struct NodeObj{
    struct NodeObj* next;
    struct NodeObj* prev;
    LIST_ELEMENT val;
};
typedef struct NodeObj* Node;
Node createNode(LIST_ELEMENT val){
    Node newNode = (Node)malloc(sizeof(struct NodeObj));
    if(!newNode){
        return NULL;
    }
    /*
    newNode->next = NULL;
    newNode->prev = NULL;
    */
    newNode->val = val;
    return newNode;
}
Node deleteNodeGetNext(Node n){
    if(!n){
        return NULL;
    }
    Node nextNode = n->next;
    free(n);
    return nextNode;
}
void deleteNodes(Node head){
    Node temp = head;
    while(temp){
        temp = deleteNodeGetNext(temp);
    }
}
struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int index;
    int length;
};

List newList(){
    List nl = (List)malloc(sizeof(struct ListObj));
    if(nl == NULL){
        printf("List Error: Not enough heap memory for allocation.\n");
        return NULL;
    }
    nl->index = LIST_CURSOR_UNDEFINED;
    nl->length = 0;
    nl->front = NULL;
    nl->back = NULL;
    nl->cursor = NULL;
    return nl;
}
void freeList(List* pL){
    nullGuard(pL,"freeList");
    List l = *pL;
    if(!l){
        return;
    }
    deleteNodes(l->front);
    free(l);
    l = NULL;
}
int length(List l){
    nullGuard(l,"length");
    return l->length;
}
int index(List l){
    nullGuard(l,"index");
    return l->index;
}
LIST_ELEMENT front(List l){
    nullGuard(l,"front");
    if(l->length <= 0){
        error("front","an empty list");
    }
    return l->front->val;
}
LIST_ELEMENT back(List l){
    nullGuard(l,"back");
    if(l->length <= 0){
        error("back","an empty list");
    }
    return l->back->val;
}
LIST_ELEMENT get(List l){
    nullGuard(l,"get");
    if(l->length <= 0){
        error("get","an empty list");
    }
    if(l->index == LIST_CURSOR_UNDEFINED){
        error("get","an uninitialized cursor");
    }
    return l->cursor->val;
}
int list_equals(List a, List b){
    if(!a){
        error("equals","null \"A\" ptr");
    }
    if(!b){
        error("equals","null \"B\" ptr");
    }
    if(a->length != b->length){
        return 0;
    }
    Node aCurr = a->front, bCurr = b->front;
    while(aCurr){
        if(aCurr->val != bCurr->val){
            return 0;
        }
        aCurr = aCurr->next;
        bCurr = bCurr->next;

    }
    return 1;
}

void clear(List l){
    nullGuard(l,"clear");
    deleteNodes(l->front);
    l->front = NULL;
    l->back = NULL;
    l->index = -1;
    l->length = 0;
}
void set(List l, LIST_ELEMENT x){
    nullGuard(l,"set");
    if(l->length == 0){
        error("set","empty list");
    }
    if(l->index == LIST_CURSOR_UNDEFINED){
        error("set","an uninitialized cursor");
    }
    l->cursor->val = x;
}
void moveFront(List l){
    nullGuard(l,"moveFront");
    if(l->length == 0){
        return;
    }
    l->index = 0;
    l->cursor = l->front;
}
void moveBack(List l){
    nullGuard(l,"moveFront");
    if(l->length == 0){
        return;
    }
    l->index = l->length - 1;
    l->cursor = l->back;
}
void movePrev(List l){
    nullGuard(l,"movePrev");
    if(l->index == LIST_CURSOR_UNDEFINED){
        return;
    }
    if(l->index == 0){
        l->index = LIST_CURSOR_UNDEFINED;
        return;
    }
    l->index -= 1;
    l->cursor = l->cursor->prev;
}
void moveNext(List l){
    nullGuard(l,"movePrev");
    if(l->index == LIST_CURSOR_UNDEFINED){
        return;
    }
    if(l->index == l->length - 1){
        l->index = LIST_CURSOR_UNDEFINED;
        return;
    }
    l->index += 1;
    l->cursor = l->cursor->next;
}


void prepend(List l, LIST_ELEMENT x){
    nullGuard(l,"prepend");
    Node insert = createNode(x);
    if(!insert){
        error("prepend","a full heap");
    }
    insert->prev = NULL;
    if(l->length > 0){
        insert->next = l->front;
        l->front->prev = insert;
        l->front = insert;
    }
    else{
        l->front = insert;
        l->back = insert;
        insert->next = NULL;
    }
    if(l->index != LIST_CURSOR_UNDEFINED){
        l->index += 1;
    }
    l->length += 1;
}
void append(List l, LIST_ELEMENT x){
    nullGuard(l,"append");
    Node insert = createNode(x);
    if(!insert){
        error("append","a full heap");
    }

    insert->next = NULL;
    if(l->length > 0){
        insert->prev = l->back;
        l->back->next = insert;
        l->back = insert;
    }
    else{
        l->front = insert;
        l->back = insert;
        insert->prev = NULL;
    }
    l->length += 1;
}
void insertBefore(List l, LIST_ELEMENT x){
    nullGuard(l,"insertBefore");
    if(l->length == 0){
        error("insertBefore", "an empty list");
    }
    if(l->index == -1){
        error("insertBefore","an uninitialized cursor");
    }
    Node insert = createNode(x);
    Node cursor = l->cursor;
    if(cursor->prev){
        insert->prev = cursor->prev;
        insert->next = cursor;
        cursor->prev->next = insert;
        cursor->prev = insert;
    }
    else{
        insert->prev = NULL;
        insert->next = cursor;
        cursor->prev = insert;
        l->front = insert;
    }
    l->length += 1;
    l->index += 1;
}
void insertAfter(List l, LIST_ELEMENT x){
    nullGuard(l,"insertAfter");
    if(l->length == 0){
        error("insertAfter", "an empty list");
    }
    if(l->index == -1){
        error("insertAfter","an uninitialized cursor");
    }
    Node insert = createNode(x);
    Node cursor = l->cursor;
    if(cursor->next){
        insert->next = cursor->next;
        insert->prev = cursor;
        cursor->next->prev = insert;
        cursor->next = insert;

    }
    else{
        insert->next = NULL;
        insert->prev = cursor;
        cursor->next = insert;
        l->back = insert;
    }
    l->length += 1;
}

void deleteFront(List l){
    nullGuard(l,"deleteFront");
    if(l->length == 0){
        error("deleteFront","empty list");
    }

    if(l->length == 1){
        clear(l);
        return;
    }
    else{
        Node front = l->front;
        front->next->prev = NULL;
        l->front = front->next;
        free(front);

    }
    l->length -= 1;
    if(l->index == 0){
        l->index = LIST_CURSOR_UNDEFINED;
    }
    else{
        l->index -= 1;
    }

}
void deleteBack(List l){
    nullGuard(l,"deleteBack");
    if(l->length == 0){
        error("deleteBack","empty list");
    }

    if(l->length == 1){
        clear(l);
        return;
    }
    else{
        Node back = l->back;
        back->prev->next = NULL;
        l->back = back->prev;
        free(back);

    }
    l->length -= 1;
    if(l->index == l->length){
        l->index = LIST_CURSOR_UNDEFINED;
    }
}
void delete(List l){
    nullGuard(l,"delete");
    if(l->length == 0){
        error("delete","empty list");
    }
    if(l->index == LIST_CURSOR_UNDEFINED){
        error("delete","an uninitialized cursor");
    }

    if(l->cursor == l->front){
        l->front = l->cursor->next;
    }
    if(l->cursor == l->back){
        l->back = l->cursor->prev;
    }
    Node cursorPrev = l->cursor->prev;
    Node cursorNext = l->cursor->next;
    if(cursorPrev){
        cursorPrev->next = cursorNext;
    }
    if(cursorNext){
        cursorNext->prev = cursorPrev;
    }
    free(l->cursor);
    l->index = LIST_CURSOR_UNDEFINED;
    l->length -= 1;
}

void printList(FILE* out, List l){
    nullGuard(l,"printList");
    Node currNode = l->front;
    while(currNode){
        if(currNode->next){
            fprintf(out, "%"LIST_FORMAT,currNode->val);
            fprintf(out, " ");
        }
        else{
            fprintf(out,"%"LIST_FORMAT,currNode->val);
        }
        currNode = currNode->next;
    }
}
List copyList(List l){
    List l2 = newList();
    Node currNode = l->front;
    while(currNode){
        append(l2,currNode->val);
        currNode = currNode->next;
    }
    return l2;
}

