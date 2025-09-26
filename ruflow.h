#ifndef RUFLOW_H
#define RUFLOW_H

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
    LA, IN, OUT, NEW, HELP, F
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

void help();
void getCurrentTime(int type, char *buff);
int checkExistingFile(char *filename);
int getValue(char *filename, valyu *new0);
int getSavings(char *filename, valyu *new2);

#endif // RUFLOW_H

