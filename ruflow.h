#ifndef AWOKFLOW_H
#define AWOKFLOW_H

typedef struct {

    int amount;
    int savings;

    char date[11];
    char clock[9];
    char *name;
    char activity[9];
    char *desc;
    
} valyu;

enum {
    LA, IN, OUT, NEW
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

void help();
void getCurrentTime(int type, char *buff);
int checkExistingFile(char *filename);
int getValue(char *filename, valyu *new0);
int getSavings(char *filename, valyu *new2);

#endif // AWOKFLOW_H

