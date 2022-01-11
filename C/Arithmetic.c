/*
 * Sam Wortzman
 * CruzID: swortzma
 * Assignment: pa7
 */

#include "BigInteger.h"
#include <stdlib.h>
#include <string.h>

char* getLineEZ(FILE* fileIn){
    int count = 0;
    int c = fgetc(fileIn);
    if(c == EOF){
        return NULL;
    }
    int size = 5;
    char* bfr = (char*)malloc(sizeof(char)*size);
    while(!feof(fileIn)){
        if(count == size - 1){
            bfr = (char*)realloc(bfr,size=size*2);
        }
        if(c == '\n'){
            break;
        }
        bfr[count++] = c;
        c = fgetc(fileIn);
    }
    bfr[count] = '\0';
    return bfr;
}

void extractBigIntsFromFile(FILE* in, BigInteger* arr){
    char* trash1 = getLineEZ(in);
    char* str1 = getLineEZ(in);
    char* trash2 = getLineEZ(in);
    char* str2 = getLineEZ(in);
    if(!str1 || !str2){
        free(trash1);
        free(trash2);
        free(str1);
        free(str2);
        fprintf(stderr,"input file invalid\n");
        return;
    }
    arr[0] = stringToBigInteger(str1);
    arr[1] = stringToBigInteger(str2);
    free(trash1);
    free(trash2);
    free(str1);
    free(str2);
}

//arr is a 10 long array with A = arr[0] and B = arr[1]
void doCalculations(BigInteger* arr){
    BigInteger A = arr[0], B = arr[1];
    BigInteger temp = newBigInteger();
    BigInteger bi2 = stringToBigInteger("2");
    BigInteger bi3 = stringToBigInteger("3");
    BigInteger bi9 = stringToBigInteger("9");
    BigInteger bi16 = stringToBigInteger("16");

    // A + B
    arr[2] = sum(A,B);

    // A - B
    arr[3] = diff(A,B);

    // A - A
    arr[4] = diff(A,A);

    // 3A - 2B
    arr[5] = prod(bi3,A);
    multiply(temp,bi2,B);
    //printBigInteger(stdout,temp);
    //puts("");
    //printBigInteger(stdout, arr[5]);
    //puts("");
    subtract(arr[5],arr[5],temp);
    //printBigInteger(stdout, arr[5]);
    //puts("");
    // A*B
    arr[6] = prod(A,B);

    // A**2
    arr[7] = prod(A,A);

    // B**2
    arr[8] = prod(B,B);

    //9(A**4) + 16(B**5)
    arr[9] = prod(arr[7],arr[7]); //(a**2)**2
    multiply(arr[9],arr[9],bi9); 
    //printBigInteger(stdout, arr[9]);
    //puts("");
    multiply(temp,arr[8],arr[8]); //(b**2)**2 * B == B**5
    //printBigInteger(stdout, temp);
    //puts("");
    multiply(temp,temp,B);
    //printBigInteger(stdout, temp);
    //puts("");
    multiply(temp,temp,bi16);
    //printBigInteger(stdout, temp);
    //puts("");

    add(arr[9],arr[9],temp);

    freeBigInteger(&temp);
    freeBigInteger(&bi2);
    freeBigInteger(&bi3);
    freeBigInteger(&bi9);
    freeBigInteger(&bi16);
}
void printBigInts(FILE* out, BigInteger* arr, int len){
    printBigInteger(out,arr[0]);
    for(int i = 1; i < len; ++i){
        fprintf(out,"\n\n");
        printBigInteger(out,arr[i]);
    }
}

void freeBigInts(BigInteger* arr, int len){
    for(int i = 0; i < len; ++i){
        freeBigInteger(arr+i);
    }
}

int main(int argc, char** argv){
    
    if(argc != 3){
        fprintf(stderr,"Usage error: Requires exactly 2 args, %d were given\n",argc - 1);
        exit(1);
    }
    char* strIn = argv[1];
    char* strOut = argv[2];
    
    FILE* fileIn = fopen(strIn,"r");
    if(!fileIn){
        fprintf(stderr,"Could not open file %s",strIn);
        exit(1);
    }
    const int len = 10;
    BigInteger bigIntegerArr[len];
    memset(bigIntegerArr,0,len*sizeof(BigInteger)); //may not be needed

    extractBigIntsFromFile(fileIn,bigIntegerArr);
    fclose(fileIn);
    doCalculations(bigIntegerArr);
    
    FILE* fileOut = fopen(strOut,"w");
    if(!fileOut){
        fprintf(stderr,"Could not open file %s",strOut);
        freeBigInts(bigIntegerArr,len);
        exit(1);
    }
    printBigInts(fileOut,bigIntegerArr,len);

    fclose(fileOut);
    freeBigInts(bigIntegerArr,len);

    return EXIT_SUCCESS;
}