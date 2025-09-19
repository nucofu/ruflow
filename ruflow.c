#include "awokflow.h"

void getCurrentTime(int value, char *buff) {
    time_t curtime;
    struct tm *timeinfo;
    
    time(&curtime);
    timeinfo = localtime(&curtime);

    if (value == 1) {
        strftime(buff, 11, "%d-%m-%Y", timeinfo);
    } else if (value == 0) {
        strftime(buff, 9, "%H:%M:%S", timeinfo);
    }
}

int checkExistingFile(char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    if (file != NULL) {
        return 1;
    }
    return 0;
}

int getValue(char *filename, valyu *new0) {
    if (checkExistingFile(filename) == 0) {
        return 1;
    } else {
        char buff[512];
        FILE *logfile;
        logfile = fopen(filename, "r");
        while (fgets(buff, sizeof(buff), logfile) != NULL) {
            if (sscanf(buff, "%s, %s, Savings: %d", new0->date, new0->name, &new0->savings) != 3) {
                sscanf(buff, "[%s] %s cash, amount: Rp%d, description: %s, savings: Rp%d\n", new0->clock, new0->activity, &new0->amount, new0->desc, &new0->savings);
            }
        }
        return 0;
    }

}

int getSavings(char *filename, valyu *new2) {
    getValue(filename, new2);
    return new2->savings;
}

int main(int argc, char *argv[]) {
    FILE *logfile;
    valyu data[4];
    char *filename;
    int gSavings;
    int seehelp = 0;

    for (int i = 1; i < argc; i++) {

        if (argv[i][0] != '-') {
            filename = argv[i];
            gSavings = getSavings(filename, &data[LA]);
        } else if (argv[i][0] == '-') {
            if (argv[i][1] == 'h') {
                puts("help");
                
            } else if (argv[i][1] == 'l') {

                i++;
                if (getValue(argv[i], &data[LA]) == 0) {
                    printf("[%s] %s cash\n amount: Rp%d\ndescription: %s\nsavings: %d\n", data[LA].clock, data[LA].activity, data[LA].amount, data[LA].desc, data[LA].savings);
                }
                
                // puts("Last Activity");
                
            } else if (argv[i][1] == 'i') {
                getCurrentTime(0, data[IN].clock);

                // amount of money
                i++;
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    data[IN].amount = atoi(argv[i]);
                }
                
                //short description
                i++; // can di test kabeh i++
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    data[IN].desc = argv[i];
                }
                
                logfile = fopen(filename, "a");

                gSavings += data[IN].amount;
                fprintf(logfile, "[%s] incoming cash, amount: Rp%d, description: %s, savings: Rp%d\n", data[IN].clock, data[IN].amount, data[IN].desc, gSavings);
                fclose(logfile);
                
                // puts("incoming cash");
                
            } else if (argv[i][1] == 'o') {
                getCurrentTime(0, data[OUT].clock);

                // amount of money
                i++;
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    data[OUT].amount = atoi(argv[i]);
                }

                
                //short description
                i++; // can di test kabeh i++
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    data[OUT].desc = argv[i];
                }
                
                logfile = fopen(filename, "a");

                gSavings -= data[OUT].amount;
                fprintf(logfile, "[%s] outgoing cash, amount: Rp%d, description: %s, savings: Rp%d\n", data[OUT].clock, data[OUT].amount, data[OUT].desc, gSavings);
                fclose(logfile);
                
                // puts("outgoing cash");
                
            } else if (argv[i][1] == 'n') {
                getCurrentTime(1, data[NEW].date);
                int saving;
                
                i++;
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    data[NEW].name = argv[i];
                }
                
                i++;
                if (argv[i] == NULL) {
                    seehelp = 1;
                    break;
                } else {
                    saving = atoi(argv[i]);
                }

                logfile = fopen(filename, "w");
                fprintf(logfile, "%s, %s, Savings: %d", data[NEW].date, data[NEW].name, saving);
                fclose(logfile);

                // puts("newfile");
                
            } else {
                seehelp = 1;
                break;
            }
            
        }
    }

    if (seehelp == 1) {
        puts("see log");
    }
    
    return 0;
}
