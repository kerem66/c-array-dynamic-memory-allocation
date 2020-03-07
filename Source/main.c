#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void getArg1(int arg1Arr[], char testvar[]){ /* gets the array and first argument to fix sizes of the mapmatrix with that I hold the size in an array*/
    char *token = strtok(testvar, "x");
    int arg1counter=0;
    while (token != NULL)
    {
        arg1Arr[arg1counter]=atoi(token);
        token = strtok(NULL, "x");
        arg1counter++;
    }
    free(testvar);

}
void getKeyMatrix(FILE *fp, int** ptArr, int size){
    int i,j;
    for(i=0;i<size;i++){
        for(j=0; j<size;j++){
            fscanf(fp,"%d", &ptArr[i][j]);
        }
    }
}

void getMapMatrix(FILE *fp, int** ptArr, int rows,int columns){
    int i,j;
    for(i=0;i<rows;i++){
        for(j=0; j<columns;j++){
            fscanf(fp,"%d", &ptArr[i][j]);
        }
    }
}

void findTreasure(int** map, int** key,int rowsize, int columnsize, int keysize,int startPointRow,int startPointColumn, FILE *outfp){
    int i,j,sum,mid;
    sum=0;
    mid=(keysize-1)/2; /* to find mid point of mapmatrix everytime*/
    int startPointRow1=startPointRow;
    int startPointColumn1=startPointColumn;
    for(i=0; i<keysize; startPointRow1++){
        for(j=0,startPointColumn1=startPointColumn; j<keysize; startPointColumn1++){
            sum+= (map[startPointRow1][startPointColumn1]) * (key[i][j]);
            j++;
        }
        i++;
    }
    /* 0: Found treasure
        1: Go up
        2: Go down
        3: Go right
        4: Go left*/
    int division=sum%5;
    fprintf(outfp,"%d;%d:%d\n",(startPointRow+mid),(startPointColumn+mid),sum); /*printing to output file*/
    if(division==0){
        exit(1);
    }else if(division==1){                                                                                      /* from there checking what is remainder and by direction doing necessary addition or subtraction */
        startPointRow-=keysize;
        if(startPointRow<0){  /* out of index in mapmaptrix*/
            startPointRow+= 2*keysize;                                                                                  /* so not be able to out of bounds going back in reverse direction and doing same thing for all other possibilities*/
            findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
        }
        findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
    }else if(division==2){
        startPointRow+=keysize;
        if(startPointRow>rowsize-1){
            startPointRow-= 2*keysize;
            findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
        }
        findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
    }else if(division==3){
        startPointColumn+=keysize;
        if(startPointColumn>columnsize-1){
            startPointColumn-= 2*keysize;
            findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
        }
        findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
    }else if(division==4){
        startPointColumn-=keysize;
        if(startPointColumn<0){
            startPointColumn+= 2*keysize;
            findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
        }
        findTreasure(map,key,rowsize,columnsize,keysize,startPointRow,startPointColumn,outfp);
    }
}

int main(int argc, char *argv[])
{
    int **map,**key;
    int mapMatrixCounter,keyMatrixCounter;
    size_t len = strlen(argv[1]); /* getting the length of first argument*/
    char * testvar = malloc(len+1); /* allocating memory*/
    if(testvar==NULL)
    {
        printf("allocation failed"); /* allocation failed */
    }
    strcpy(testvar, argv[1]); /* testvar now has the first argument*/
    int arg1[2];/* array to hold first argument*/
    getArg1(arg1, testvar);
    int keyMapSize= atoi(argv[2]);
    FILE *mapfp; /*map matrix pointer*/
    mapfp=fopen(argv[3],"r"); /* taking mapmatrix to read*/
    FILE *keyfp;
    FILE *outfp;
    outfp=fopen(argv[5],"w"); /*opening file to write*/
    keyfp=fopen(argv[4],"r"); /* takin keymap to read*/

    map= malloc(arg1[0] * sizeof *map);
    for(mapMatrixCounter=0; mapMatrixCounter<arg1[0];mapMatrixCounter++){
        map[mapMatrixCounter]=malloc(arg1[1] * sizeof *map[mapMatrixCounter]);           /* allocating memory for map matrix */
    }
    getMapMatrix(mapfp,map, arg1[0],arg1[1] );

    key= malloc(keyMapSize * sizeof *key);                                                 /* allocating memory for key matrix*/
    for(keyMatrixCounter=0;keyMatrixCounter<keyMapSize;keyMatrixCounter++){
        key[keyMatrixCounter]=malloc(keyMapSize * sizeof *key[keyMatrixCounter]);
    }
    getKeyMatrix(keyfp,key,keyMapSize);
    findTreasure(map,key,arg1[0],arg1[1],keyMapSize,0,0,outfp);

    for (keyMatrixCounter=0; keyMatrixCounter<arg1[0]; keyMatrixCounter++) /* freeing memory for keymatrix*/
    {
        free(map[keyMatrixCounter]);
    }
    free(key);

    for (mapMatrixCounter=0; mapMatrixCounter<arg1[0]; mapMatrixCounter++) /* freeing memory for mapmatrix*/
    {
        free(map[mapMatrixCounter]);
    }
    free(map);
    fclose(outfp);
    fclose(mapfp);
    fclose(keyfp);
    return 0;
}
