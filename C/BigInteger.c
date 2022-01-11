
/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa7
 */

#include "BigInteger.h"
#include "List.h"
#include "inttypes.h"
#include "stdlib.h"

#define MAX_L 1000000000l
#define MAX_B 9

#define IntError(op,pre) fprintf(stderr,"BigInt Error: Calling %s on %s\n",op,pre); exit(1);
#define IntNullGuard(BigInt,op) if(!(BigInt)) {IntError(op,"null ptr");}

struct BigIntegerObj{
    int8_t sign;
    List num;
};

BigInteger newBigInteger(){
    BigInteger res = (BigInteger)malloc(sizeof(struct BigIntegerObj));
    res->sign = 0;
    res->num = newList();
    return res;
}

LIST_ELEMENT pow10(int n){
    int res = 1;
    for(int i = 0; i < n; ++i){
        res *= 10;
    }
    return res;
}
void BigIntFixup(BigInteger B){
    List num = B->num;
    LIST_ELEMENT carry = 0,mod;
    for(moveFront(num); index(num) != LIST_CURSOR_UNDEFINED; moveNext(num)){
        carry = get(num) + carry;
        
        mod = carry % MAX_L;
        carry /= MAX_L;
        set(num,mod);
    }
    if(carry > 0){
        append(num,carry);
    }
}

BigInteger stringToBigInteger(char* s){
    if(!s){
        IntError("stringToBigInteger","null char*");
    }
    BigInteger res = newBigInteger();
    List num = res->num;
    char c;
    LIST_ELEMENT nPos = -1;
    int end = 0;
    for (LIST_ELEMENT i = 0; (c = s[i]) != '\0'; ++i) {
        if (i == 0) {
            if(c == '-'){
                res->sign = -1;
                end = 1;
            }
            else if(c == '+'){
                end = 1;
            }
            else if(c >= '0' && c <= '9'){
                ++nPos;
            }
        }
        else if (c >= '0' && c <= '9') {
            ++nPos;
        }
        else{
            IntError("StrToBigInt","Non-Digit number");
        }
    }
    if(nPos < 0){
        IntError("StrToBigInt","Empty Integer");
    }
    int extra = (nPos+1) % MAX_B;
    nPos += end;
    LIST_ELEMENT n = 0;
    int d = 0;

    while (nPos >= extra + end) {
        char digit = s[nPos--];
        n += (digit - '0') * pow10(d++);
        if (d == MAX_B) {
            
            append(num,n);
            
            d = 0;
            n = 0;
        }
    }
    for (int i = 0; i < extra; ++i) {
        char digit = s[nPos--];
        
        if (digit > '9' || digit < '0') {
            printf("oh no %d\n", digit);
            continue;
        }
        n += (digit - '0') * pow10(i);
    }

    append(num,n);  
    
    while(length(num) && back(num) == 0) {
        deleteBack(num);
    }
    if(length(num)){
        if(res->sign != -1){
            res->sign = 1;
        }
    }
    else{
        res->sign = 0;
    }
    return res;
    

}

void freeBigInteger(BigInteger* pN){
    if(!pN){
        return;
    }
    BigInteger N = *pN;
    if(!N){
        *pN = NULL;
        return;
    }
    freeList(&N->num);
    free(N);
    *pN = NULL;
}

void copyInto(BigInteger A, BigInteger B);

BigInteger copy(BigInteger other){
    IntNullGuard(other,"copy");
    BigInteger res = newBigInteger();
    copyInto(res,other);
    return res;
}

int compareMagnitude(BigInteger A, BigInteger B){
    if(A == B){
        return 0;
    }
    List a = A->num, b = B->num;
    if(length(a) != length(b)){
        return length(a) > length(b) ? 1 : -1;
    }
    moveBack(a);
    moveBack(b);
    while(index(a) != LIST_CURSOR_UNDEFINED){
        
        if(get(a) != get(b)){
            if(get(a) > get(b)){
                return 1;
            }
            return -1;
        }
        movePrev(a);
        movePrev(b);
    }
    return 0;

}

int compare(BigInteger A, BigInteger B){
    IntNullGuard(A,"compare");
    IntNullGuard(B,"compare");
    if(A == B){
        return 0;
    }
    int8_t aSign = A->sign, bSign = B->sign;
    if( aSign == 0 || bSign == 0){
        //return a if not b or negative b if not a
        return aSign ? aSign : bSign * -1;
    }
    if(aSign != bSign){
        //return if a's sign is greater of less than b's sign
        return aSign > bSign ? 1 : -1;
    }

    //flip the result of compareMagnitude if A and B are negative
    return compareMagnitude(A,B) * A->sign;
}



//sets the sum of max and min into max
void add_unchecked(BigInteger max, BigInteger min){
    List maxNum = max->num, minNum = min->num;
    LIST_ELEMENT both, carry = 0;
    moveFront(maxNum);
    moveFront(minNum);
    while (index(minNum) != LIST_CURSOR_UNDEFINED) {
        both = get(maxNum) + get(minNum) + carry;
        carry = both / MAX_L;
        set(maxNum,both % MAX_L);
        moveNext(maxNum);
        moveNext(minNum);
    }
    while (index(maxNum) != LIST_CURSOR_UNDEFINED) {
        both = get(maxNum) + carry;
        carry = both / MAX_L;
        set(maxNum,both % MAX_L);
        moveNext(maxNum);
        if (!carry) {
            return;
        }
    }
    if (carry) {
        append(maxNum,carry);
    }
}



void sub_unchecked(BigInteger max, BigInteger min){
    List maxNum = max->num, minNum = min->num;
    LIST_ELEMENT both, borrow = 0;
    moveFront(maxNum);
    moveFront(minNum);
    while (index(minNum) != LIST_CURSOR_UNDEFINED) {
        both = get(maxNum) - get(minNum) - borrow;
        borrow = 0;
        if (both < 0){
            borrow = 1;
            both = MAX_L + both;
        }
        set(maxNum,both);
        moveNext(maxNum);
        moveNext(minNum);
    }
    while (index(maxNum) != LIST_CURSOR_UNDEFINED) {
        both = get(maxNum) - borrow;
        borrow = 0;
        if (both < 0){
            borrow = 1;
            both = MAX_L + both;
        }
        set(maxNum,both);
        moveNext(maxNum);
    }
    if (back(maxNum) == 0) {

        deleteBack(maxNum);
        
        if(length(maxNum) == 0){
            max->sign = 0;
        }
    }
    
}

BigInteger sum(BigInteger lhs, BigInteger rhs){
    IntNullGuard(lhs,"sum");
    IntNullGuard(rhs,"sum");
    BigInteger max = length(lhs->num) > length(rhs->num) ? lhs : rhs;
    BigInteger min = length(lhs->num) <= length(rhs->num) ? lhs : rhs;
    BigInteger res;
    if(max->sign == min->sign){
        res = copy(max);
        add_unchecked(res,min);
        return res;
    }
    else{
        if(length(max->num) == length(min->num)){
            int cmp = compareMagnitude(max,min);
            if(cmp < 0){
                BigInteger temp = min;
                min = max;
                max = temp;
            }
            else if(cmp == 0){
                res = newBigInteger();
                return res;
            }
        }
        res = copy(max);
        sub_unchecked(res,min);
        return res;
    }
}
//copies B into preexisting BigInt A
void copyInto(BigInteger A, BigInteger B){
    A->sign = B->sign;
    List a = A->num, b = B->num;
    moveFront(a);
    moveFront(b);
    int lenA = length(a), lenB = length(b);
    if(lenA >= lenB){
        while(index(b) != LIST_CURSOR_UNDEFINED){
            set(a,get(b));
            moveNext(a);
            moveNext(b);
        }
        while(index(a) != LIST_CURSOR_UNDEFINED){
            deleteBack(a);
        }
    }
    else{
        while(index(a) != LIST_CURSOR_UNDEFINED){
            set(a,get(b));
            moveNext(a);
            moveNext(b);
        }
        while(index(b) != LIST_CURSOR_UNDEFINED){
            append(a,get(b));
            moveNext(b);
        }
    }
}

void add(BigInteger P, BigInteger A, BigInteger B){
    IntNullGuard(A,"subtract");
    IntNullGuard(B,"subtract");
    IntNullGuard(P,"subtract");
    BigInteger max = length(A->num) > length(B->num) ? A : B;
    BigInteger min = length(A->num) <= length(B->num) ? A : B;
    if(max->sign == min->sign){
        BigInteger temp = P;
        if(P == max || P == min){
            temp = newBigInteger();
        }
        copyInto(temp,max);
        add_unchecked(temp,min);
        temp->sign = A->sign;
        if(P != temp){
            copyInto(P,temp);
            freeBigInteger(&temp);
        }
        return;
    }
    else{
        if(length(max->num) == length(min->num)){
            int cmp = compareMagnitude(max,min);
            if(cmp < 0){
                BigInteger temp = min;
                min = max;
                max = temp;
            }
            else if(cmp == 0){
                clear(P->num);
                P->sign = 0;
                return;
            }
        }
        BigInteger temp = P;
        if(P == max || P == min){
            temp = newBigInteger();
        }
        copyInto(temp,max);
        sub_unchecked(temp,min);
        
        if(temp != P){
            copyInto(P,temp);
            freeBigInteger(&temp);
        }
        return;
    }
}

void subtract(BigInteger P, BigInteger A, BigInteger B){
    IntNullGuard(A,"subtract");
    IntNullGuard(B,"subtract");
    IntNullGuard(P,"subtract");
    BigInteger max = length(A->num) > length(B->num) ? A : B;
    BigInteger min = length(A->num) <= length(B->num) ? A : B;
    if(max->sign != min->sign){
        BigInteger temp = P;
        if(P == max || P == min){
            temp = newBigInteger();
        }
        copyInto(temp,max);
        add_unchecked(temp,min);
        if(max == B){
            temp->sign *= -1;
        }
        if(P != temp){
            copyInto(P,temp);
            freeBigInteger(&temp);
        }
        return;
    }
    else{
        if(length(max->num) == length(min->num)){
            int cmp = compareMagnitude(max,min);
            if(cmp < 0){
                BigInteger temp = min;
                min = max;
                max = temp;
            }
            else if(cmp == 0){
                clear(P->num);
                P->sign = 0;
                return;
            }
        }
        BigInteger temp = P;
        if(P == max || P == min){
            temp = newBigInteger();
        }
        copyInto(temp,max);
        sub_unchecked(temp,min);
        if(max == B){
            temp->sign *= -1;
        }
        if(temp != P){
            copyInto(P,temp);
            freeBigInteger(&temp);
        }
        return;
    }
}

BigInteger diff(BigInteger lhs, BigInteger rhs){
    IntNullGuard(lhs,"diff");
    IntNullGuard(rhs,"diff");
    
    BigInteger max = length(lhs->num) > length(rhs->num) ? lhs : rhs;
    BigInteger min = length(lhs->num) <= length(rhs->num) ? lhs : rhs;
    BigInteger res;
    if(max->sign != min->sign){
        res = copy(max);
        add_unchecked(res,min);
        res->sign = lhs->sign;
        return res;
    }
    else{
        if(length(max->num) == length(min->num)){
            int cmp = compareMagnitude(max,min);
            if(cmp < 0){
                BigInteger temp = min;
                min = max;
                max = temp;
            }
            else if(cmp == 0){
                res = newBigInteger();
                return res;
            }
        }
        res = copy(max);
        sub_unchecked(res,min);
        if(max == rhs){
            res->sign *= -1;
        }
        return res;
    }
}

void addListNumber(List L, int n, LIST_ELEMENT x){
    int len = length(L);
    
    if(len == n){
        append(L,x);
        return;
    }
    int i = index(L);
    if(i == LIST_CURSOR_UNDEFINED){
        int len = length(L);
        if(n > len/2){
            moveBack(L);
            i = len - 1;
        }
        else{
            moveFront(L);
            i = 0;
        }
    }
    if(i > n){
        for(int j = 0; j < i-n; ++j){
            movePrev(L);
        }
    }
    else if( i < n){
        for(int j = 0; j < n-i; ++j){
            moveNext(L);
        }
    }
    set(L,x + get(L));
}
void multiply(BigInteger P, BigInteger A, BigInteger B){
    IntNullGuard(A,"multipyly");
    IntNullGuard(B,"multiply");
    IntNullGuard(P,"multiply");
    int8_t mix = A->sign * B->sign;
    if(mix == 0){
        clear(P->num);
        P->sign = 0;
        return;
    }
    if(P == A || P == B){
        BigInteger res = prod(A,B);
        copyInto(P,res);
        freeBigInteger(&res);
        return;
    }
    List a = A->num, b = B->num, r = P->num;
    for(moveFront(r); index(r) != LIST_CURSOR_UNDEFINED; moveNext(r)){
        set(r,0);
    }
    int freeCopy = 0;
    if(a == b){
        b = copyList(b);
        freeCopy = 1;
    }
    int trailing = 0;
    moveFront(a);
    while(index(a) != LIST_CURSOR_UNDEFINED){
        int i = 0;
        moveFront(b);
        moveFront(r);
        LIST_ELEMENT currVal = get(a);
        while(index(b) != LIST_CURSOR_UNDEFINED){
            LIST_ELEMENT p = currVal * get(b);
            addListNumber(r,i+trailing,p);
            moveNext(b);
            ++i;
        }
        ++trailing;
        moveNext(a);
        BigIntFixup(P);
    }
    P->sign = mix;
    if(freeCopy){
        freeList(&b);
    }
    while(back(r) == 0){
        deleteBack(r);
    }
}
BigInteger prod(BigInteger A, BigInteger B){
    IntNullGuard(A,"prod");
    IntNullGuard(B,"prod");
    BigInteger res = newBigInteger();
    int8_t mix = A->sign * B->sign;
    if(mix == 0){
        return res;
    }
    List a = A->num, b = B->num, r = res->num;
    int freeCopy = 0;
    if(a == b){
        b = copyList(b);
        freeCopy = 1;
    }
    int trailing = 0;
    //printList(stdout,a);
    moveFront(a);
    while(index(a) != LIST_CURSOR_UNDEFINED){
        int i = 0;
        moveFront(b);
        moveFront(r);
        LIST_ELEMENT currVal = get(a);
        while(index(b) != LIST_CURSOR_UNDEFINED){
            LIST_ELEMENT p = currVal * get(b);
            
            //printf("%d\n",i+trailing); //remove later---------
            addListNumber(r,i+trailing,p);
            moveNext(b);
            ++i;
        }
        ++trailing;
        moveNext(a);
        BigIntFixup(res);
    }
    res->sign = mix;
    if(freeCopy){
        freeList(&b);
    }
    return res;
}

int sign(BigInteger N){
    IntNullGuard(N,"sign");
    return N->sign;
}

int equals(BigInteger A, BigInteger B){
    IntNullGuard(A,"equals");
    IntNullGuard(B,"eqauls");
    return (compare(A,B) == 0);
}


void makeZero(BigInteger N){
    IntNullGuard(N,"makeZero");
    clear(N->num);
    N->sign = 0;
}

void negate(BigInteger N){
    IntNullGuard(N,"negate");
    N->sign *= -1;
}


void printBigInteger(FILE* out, BigInteger B){
    if(B->sign == 0){
        fprintf(out, "0");
        return;
    }
    List num = B->num;
    moveBack(num);
    if(B->sign == -1){
        fprintf(out,"-");
    }
    fprintf(out,"%"LIST_FORMAT,get(num));
    for(int i = length(num) - 2; i >= 0; --i){
        movePrev(num);
        fprintf(out,"%0*"LIST_FORMAT,MAX_B,get(num));
    }
}