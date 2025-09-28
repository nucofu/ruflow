#ifndef RUFLOW_H
#define RUFLOW_H

typedef struct {

    int amount;
    int savings;

    char clock[9];
    char activity[9];
    char desc[128];
    char date[11];
    char name[32];
    
} valyu;

enum {
    LA, IN, OUT, NEW, HELP, F, VER
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

void help(int ver);
void getCurrentTime(int type, char *buff);
int checkExistingFile(char *filename);
int getValue(char *filename, valyu *new0, int *flag);
int getSavings(char *filename);

#endif // RUFLOW_H

