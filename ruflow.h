#ifndef RUFLOW_H
#define RUFLOW_H

typedef struct {

    int amount;
    int savings;

    char clock[9];
    char activity[9];
    char desc[128];
    char date[11];
    char title[32];
    
} valyu;

enum {
    LA, IN, OUT, NEW, F, HELP, VER
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <ctype.h>

void help(int ver);
void getCurrentTime(int type, char *buff);
int checkExistingFile(char *filename);
int getLastActivityValue(char *filename, valyu *new0, int *flag);
int getSavings(char *filename);
int getHeader(char *filename, char date[], char title[]);

#endif // RUFLOW_H

