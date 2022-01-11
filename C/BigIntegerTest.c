
/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa7
 */

#include "BigInteger.h"
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


#define pbi(bigint) printBigInteger(stdout,bigint)


void freeBigIntegerArray(BigInteger* arr, int len){
    for(int i = 0; i < len; ++i){
        if(arr[i] == NULL){
            continue;
        }
        freeBigInteger(arr + i);
    }
}
void printArray(BigInteger* arr, int len){
    for(int i = 0; i < len; ++i){
        if(arr[i] == NULL){
            continue;
        }
        printf("%d: ",i);
        pbi(arr[i]);
        puts("");
    }
}
void pbiNice(BigInteger N){
    if(sign(N) == -1){
        printf("(");
    }
    printBigInteger(stdout,N);
    if(sign(N) == -1){
        printf(")");
    }
}
void printResultArray(BigInteger* base, int len, BigInteger* result,char* op){
    for(int i = 0; i < len; ++i){
        for (int j = 0; j < len; ++j){
            pbiNice(base[i]);
            printf(" %s ",op);
            pbiNice(base[j]);
            printf(" = ");
            printBigInteger(stdout,result[i*len + j]);
            puts("");
        }
    }   
}
//takes a base array of BigInts of length baseArrLen, and a function pointer, and applies the function 
//to every combination of A and B from the baseArr. The returning array will be baseArrLen ** 2
void testFunc2args(BigInteger* baseArr,int baseArrLen, BigInteger (*func)(BigInteger,BigInteger), char* funcName, char* op){
    BigInteger* res = (BigInteger*)malloc(sizeof(BigInteger)*baseArrLen*baseArrLen);
    for(int i = 0; i < baseArrLen; ++i){
        for (int j = 0; j < baseArrLen; ++j){
            int arrPos = i*baseArrLen + j;
            res[arrPos] = func(baseArr[i],baseArr[j]);
        }
    }

    printf("Result of %s on base array:\n",funcName);
    printResultArray(baseArr,baseArrLen,res,op);
    puts("");
    freeBigIntegerArray(res,baseArrLen*baseArrLen);
    free(res);
}


//same as testFunc2args but takes the 3 argument verb versions.
void testFunc3args(BigInteger* baseArr,int baseArrLen, void (*func)(BigInteger,BigInteger,BigInteger), char* funcName, char* op){
    BigInteger* res = (BigInteger*)malloc(sizeof(BigInteger)*baseArrLen*baseArrLen);
    for(int i = 0; i < baseArrLen * baseArrLen; ++i){
        res[i] = stringToBigInteger("-58008420691134");
    }
    for(int i = 0; i < baseArrLen; ++i){
        for (int j = 0; j < baseArrLen; ++j){
            int arrPos = i*baseArrLen + j;
            func(res[arrPos],baseArr[i],baseArr[j]);
        }
    }
    printf("\nresult of %s on Base array:\n",funcName);
    printResultArray(baseArr,baseArrLen,res,op);
    
    freeBigIntegerArray(res,baseArrLen*baseArrLen);
    free(res);
}

int main(){
    const int len = 4;
    BigInteger bases[len];
    bases[0] = stringToBigInteger("-160042587300");
    bases[1] = stringToBigInteger("1004");
    bases[2] = stringToBigInteger("-2017");
    bases[3] = stringToBigInteger("9765329462947063325");
    
    puts("base numbers:");
    printArray(bases,len);
    puts("");
    puts("TESTING NOUNS------");
    testFunc2args(bases,len,sum,"sum", "+");
    testFunc2args(bases,len,diff,"diff","-");
    testFunc2args(bases,len,prod,"prod", "*");

    puts("TESTING VERBS------");
    testFunc3args(bases,len,add,"add", "+");
    testFunc3args(bases,len,subtract,"subtract","-");
    testFunc3args(bases,len,multiply,"multiply","*");

    puts("\nTESTING OTHERS------\n");
    puts("copying a big int:");
    BigInteger cpy = copy(bases[2]);
    pbi(cpy);
    printf(" == ");
    pbi(bases[2]);
    printf(" = %d\n", equals(cpy,bases[2]));

    puts("negating copy:");
    negate(cpy);
    pbi(cpy);
    printf(" compared to ");
    pbi(bases[2]);
    printf(" = %d\n", compare(cpy,bases[2]));

    puts("clearing num:");
    makeZero(cpy);
    pbi(cpy);
    puts("");
    printf("sign of copy is now %d\n",sign(cpy));
    
    freeBigInteger(&cpy);
    assert(cpy == NULL);
    freeBigIntegerArray(bases,len);

    return EXIT_SUCCESS;
}