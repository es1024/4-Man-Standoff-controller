#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    char* input = argv[2];
    int roundCount = atoi(argv[1]);
    int enemyCount=1;
    const char delim[1] = " ";
    char *token;

    //I don't want to experience the agony of getting hit by the truck
    if(roundCount>=49){
        printf("S0");
        return 0;
    }

    token = strtok(input, delim);

    token = strtok(NULL, delim); //skip to the first enemy

    while(token != NULL){

        //if someone did nothing and is alive, take it as a tip that he will shoot next turn, and dodge.
        if(strstr(token, "N")!=NULL && strstr(token,"-")==NULL && token[0]!='0'){
            printf("D%d", enemyCount);
            return 0;
        }

        token = strtok(NULL, delim);
        enemyCount++;
    }

    //default
    printf("N");
    return 0;
}
