#include "ruflow.h"

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
        fclose(file);
        return -1;
    }
    return 0;
}

int getValue(char *filename, valyu *new0) {
    if (checkExistingFile(filename) == 0) {
        return -1;
    } else {
        char buff[512];
        FILE *logfile;
        logfile = fopen(filename, "r");
        while (fgets(buff, sizeof(buff), logfile) != NULL) {
            if (sscanf(buff, "%10s, %s, Savings: %d\n", new0->date, new0->name, &new0->savings) != 3) {
                sscanf(buff, "[%8s] %8s cash, amount: Rp%d, description: %s, savings: Rp%d\n", new0->clock, new0->activity, &new0->amount, new0->desc, &new0->savings);
            }
        }

        fclose(logfile);
        return 0;
    }

}

int getSavings(char *filename, valyu *new2) {
    getValue(filename, new2);
    return new2->savings;
}

int main(int argc, char *argv[]) {

    // multi optional variable
    FILE *logfile;
    valyu data[4];
    char *filename;
    int gSavings;

    // single optional variable
    int nSaving;
    
    // optional flag
    int flag[6];
    flag[HELP] = 0;
    flag[LA] = 0;
    flag[NEW] = 0;
    flag[IN] = 0;
    flag[OUT] = 0;
    flag[F] = 0;

    int opt;
    while ((opt = getopt(argc, argv, "f:hln:i:o:")) != -1) {
        switch(opt) {

        case 'f': {
            if (optarg != NULL) {
                filename = strdup(optarg);
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }
            
            flag[F] = 1;
            break;
        }
            
        case 'h': {
            flag[HELP] = 1;
            break;
        }
            
        case 'l': {
            flag[LA] = 1;
            break;
        }
            
        case 'n': {
            if (optarg != NULL) {
                data[NEW].name = strdup(optarg);
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }
            
            if (optind < argc) {
                nSaving = atoi(argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -n need two argument: <name> <saving>\n");
                exit(EXIT_FAILURE);
            }

            flag[NEW] = 1;
            break;
        }
        case 'i': {
            if (optarg != NULL) {
                data[IN].amount = atoi(optarg);
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc) {
                data[IN].desc = strdup(argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -i need two argument: <amount <description>\n");
                exit(EXIT_FAILURE);
            }
            
            flag[IN] = 1;                
            break;
        }
            
        case 'o': {
            if (optarg != NULL) {
                data[OUT].amount = atoi(optarg);
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc) {
                data[OUT].desc = strdup(argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -o need two argument: <amount <description>\n");
                exit(EXIT_FAILURE);
            }

            flag[OUT] = 1;                
            break;
        }

        case ':': {
            fprintf(stderr, "option -%c, need an argument, see -h for help\n", optopt);
            exit(EXIT_FAILURE);
        }
            
        case '?': {
            fprintf(stderr, "unrecognized -%c, see -h for help\n", optopt);
            exit(EXIT_FAILURE);
        }

        }
    }

    // option conflict
    if (flag[F] == 0 && (flag[LA] == 1 | flag[IN] == 1 | flag[OUT] == 1 | flag[NEW] == 1)) {
        fprintf(stderr, "option -f is required for this option (-l | -n | -i | -o)\n");
        exit(EXIT_FAILURE);
    }
    
    if (flag[HELP] == 1 && (flag[LA] == 1 | flag[NEW] == 1 | flag[IN] == 1 | flag[OUT] == 1 | flag[F] == 1)) {
        fprintf(stderr, "option -h must standalone\n");
        exit(EXIT_FAILURE);
    }

    if (flag[LA] == 1 && (flag[NEW] == 1 | flag[IN] == 1 | flag[OUT] == 1)) {
        fprintf(stderr, "option -l must standalone with -f\n");
        exit(EXIT_FAILURE);
    }

    if (flag[IN] == 1 && flag[OUT] == 1) {
        fprintf(stderr, "choose one between -i or -o\n");
        exit(EXIT_FAILURE);
    }

    // option logic           
    if (flag[F] == 1) {
        if (!filename) {
            fprintf(stderr, "invalid filename\n");
            exit(EXIT_FAILURE);
        }

        if (checkExistingFile(filename) != 0) {
            gSavings = getSavings(filename, &data[LA]);
        }
        
        // puts("filename");
    }

    if (flag[HELP] == 1) {
        puts("help");
        return 0;
    }
    
    if (flag[LA] == 1) {
        if (getValue(filename, &data[LA]) != 1) {
            printf("[%8s] %8s cash\n amount: Rp%d\ndescription: %s\nsavings: %d\n", data[LA].clock, data[LA].activity, data[LA].amount, data[LA].desc, data[LA].savings);
        } else {
            fprintf(stderr, "invalid argument\n");
            exit(EXIT_FAILURE);
        }

        // puts("Last Activity");

    }

    if (flag[NEW] == 1) {
        getCurrentTime(1, data[NEW].date);
        
        logfile = fopen(filename, "w");
        if (!logfile) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
            
        fprintf(logfile, "%10s, %s, Savings: %d\n", data[NEW].date, data[NEW].name, nSaving);
        fclose(logfile);

        // puts("newfile");

    }

    if (flag[IN] == 1) {
        getCurrentTime(0, data[IN].clock);

        logfile = fopen(filename, "a");
        if (!logfile) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
            
        gSavings += data[IN].amount;
        fprintf(logfile, "[%8s] incoming cash, amount: Rp%d, description: %s, savings: Rp%d\n", data[IN].clock, data[IN].amount, data[IN].desc, gSavings);
        fclose(logfile);

        // puts("incoming cash");

    }

    if (flag[OUT] == 1) {
        getCurrentTime(0, data[OUT].clock);

        logfile = fopen(filename, "a");
        if (!logfile) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        if (gSavings > data[OUT].amount) {
            gSavings -= data[OUT].amount;            
        } else {
            fprintf(stderr, "insufficient Savings\n");
            exit(EXIT_FAILURE);
        }
            
        fprintf(logfile, "[%8s] outgoing cash, amount: Rp%d, description: %s, savings: Rp%d\n", data[OUT].clock, data[OUT].amount, data[OUT].desc, gSavings);
        fclose(logfile);

        // puts("outgoing cash");

    }
        
    return 0;
}
