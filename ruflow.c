#include "ruflow.h"

// about

void help(int ver) {

    char desc[] = "ruflow/rupiah flow - simple cashflow manager for tracking your own personal record\n";
    
    char usage[] = "  ruflow [options]\n";
    char options[] = "  -h                             show help message\n  -v                             show versions\n  -f <file>                      creating or accessing file\n    -l                             show last activity\n    -n <saving> <title>            write new record\n    -i <amount> <description>      write incoming cash\n    -o <amount> <description>      write outgoing cash\n";
    char examples[] = "  ruflow -f cashflow.log -n 200000 \"Ucok Cashflow\"\n  ruflow -f cashflow.log -i 5000 \"from udin\"\n  ruflow -f cashflow.log -o 2000 \"from icih\"\n";
    char versions[] = "ruflow/rupiah flow - version protoype 3.2\n";

    if (ver == 1) {
        printf("%s", versions);
        exit(EXIT_SUCCESS);
    }
    
    printf("%s\nUsage:\n%s\nOptions:\n%s\nExamples:\n%s\n", desc, usage, options, examples);    
}

// check Something

int checkExistingFile(char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return -1;
    }
    return 0;
}

// get some data

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

int getLastActivityValue(char *filename, valyu *new0, int *flag) {
    FILE *logfile;
    char buff[512];
    
    if (checkExistingFile(filename) == 0) {
        return -1;
    }

    logfile = fopen(filename, "r");
    if (!logfile) {
        perror("fopen");
        return -1;
    }
    
    valyu temp;
    
    while (fgets(buff, sizeof(buff), logfile) != NULL) {
        memset(&temp, 0, sizeof(temp));
        if (sscanf(buff, "%10[^,], %31[^,], savings: Rp %d\n", temp.date, temp.title, &temp.savings) == 3)
            
            {
                *new0 = temp;
                *flag = 1;
                continue;
            }

        memset(&temp, 0, sizeof(temp));
        if (sscanf(buff, "%8[^,], %8s cash, amount: Rp %d, description: %127[^,], savings: Rp %d\n",
                   temp.clock, temp.activity, &temp.amount, temp.desc, &temp.savings) == 5)
            
            {
                *new0 = temp;
                *flag = 2;
                continue;
            }

    }
    
    fclose(logfile);
    return 0;
    
}

int getSavings(char *filename) {
    valyu temp;
    int flag;
    if (getLastActivityValue(filename, &temp, &flag) == 0) {
            return temp.savings;
    }
    return -1;
}

int getHeader(char *filename, char date[], char title[]) {
    FILE *logfile;
    char buff[512];
    
    if (checkExistingFile(filename) == 0) {
        return -1;
    }

    logfile = fopen(filename, "r");
    if (!logfile) {
        perror("fopen");
        return -1;
    }
    
    valyu temp;
    
    while (fgets(buff, sizeof(buff), logfile) != NULL) {
        memset(&temp, 0, sizeof(temp));
        if (sscanf(buff, "%10[^,], %31[^,], savings: Rp %d\n", temp.date, temp.title, &temp.savings) == 3)
            
            {
                strcpy(date, temp.date);
                strcpy(title, temp.title);
                continue;
            }
    }
    
    fclose(logfile);
    return 0;

}

// main function

int main(int argc, char *argv[]) {

    int otherFlag[5];
    otherFlag[0] = 0; // error flag if ruflow have no argument
    otherFlag[1] = 0; // error flag if argument not found
    otherFlag[2] = 0; // warning flag if only inserting -f option
    otherFlag[3] = 0; // for -n option without argument
    otherFlag[4] = 0; // warning title/description with spaces
    
    if (argc == 1) {
        otherFlag[0] = 1;
    }

    // global options variable
    FILE *logfile;
    valyu data[5];
    char *filename;

    int gSavings;
    gSavings = 0;

    char gDate[11];
    char gTitle[32];

    // options flag
    int flag[7];
    flag[HELP] = 0; // -h or Help
    flag[LA] = 0;   // -l or Last Activity
    flag[NEW] = 0;  // -n or New Record
    flag[IN] = 0;   // -i or incoming cash
    flag[OUT] = 0;  // -o or outgoing cash
    flag[F] = 0;    // -f or filename
    flag[VER] = 0;  // -v or versions

    // get an argument and send a flag
    int opt;
    while ((opt = getopt(argc, argv, "hvf:ln::i::o::")) != -1) {
        switch(opt) {

        case 'h': {
            flag[HELP] = 1;
            otherFlag[1] = 1;
            break;
        }

        case 'v': {
            flag[VER] = 1;
            otherFlag[1] = 1;
            break;
        }
            
        case 'f': {
            if (optarg != NULL) {
                filename = strdup(optarg);
            } else {
                fprintf(stderr, "fill in the file location\n");
                exit(EXIT_FAILURE);
            }
            
            flag[F] = 1;
            otherFlag[1] = 1;

            if (argv[optind] == NULL) {
                otherFlag[2] = 1;
            }
            
            break;
        }
                        
        case 'l': {
            flag[LA] = 1;
            otherFlag[1] = 1;
            break;
        }
            
        case 'n': {
            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[NEW] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    int valid = 1;
                    for (int i = 0; argv[optind][i] != '\0'; i++) {
                        if (!isdigit(argv[optind][i])) {
                            valid = 0;
                            break;
                        }
                    }
                    
                    if (valid == 1) {
                        data[NEW].savings = atoi(argv[optind]);
                    } else {
                        fprintf(stderr, "<saving> must integer\n");
                        exit(EXIT_FAILURE);
                    }
                    optind++;
                }
                
            } else {
                    if (checkExistingFile(filename) != 0) {
                        otherFlag[3] = 1;
                        flag[NEW] = 1;
                        otherFlag[1] = 1;
                        break;
                    } else {
                        fprintf(stderr, "file: %s, not found\n", filename);
                        exit(EXIT_FAILURE);
                    }
            }
            
            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[NEW] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    snprintf(data[NEW].title, sizeof(data[NEW].title), "%s", argv[optind]);
                    optind++;
                }
            } else {
                fprintf(stderr, "option -n need two argument: <saving> <title>\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc && argv[optind] != NULL && argv[optind][0] != '-') {
                otherFlag[4] = 1;
            }
            
            flag[NEW] = 1;
            otherFlag[1] = 1;
            break;
        }
            
        case 'i': {
            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[IN] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    int valid = 1;
                    for (int i = 0; argv[optind][i] != '\0'; i++) {
                        if (!isdigit(argv[optind][i])) {
                            valid = 0;
                            break;
                        }
                    }
                
                    if (valid == 1) {
                        data[IN].amount = atoi(argv[optind]);
                    } else {
                        fprintf(stderr, "<amount> must integer\n");
                        exit(EXIT_FAILURE);
                    }
                    optind++;
                }
            } else {
                fprintf(stderr, "add argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[IN] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    snprintf(data[IN].desc, sizeof(data[IN].desc), "%s", argv[optind]);
                    optind++;
                }
            } else {
                fprintf(stderr, "option -i need two argument: <amount> <description>\n");
                exit(EXIT_FAILURE);
            }
            
            if (optind < argc && argv[optind] != NULL && argv[optind][0] != '-') {
                otherFlag[4] = 1;
            }
            
            flag[IN] = 1;
            otherFlag[1] = 1;
            break;
        }
            
        case 'o': {
            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[OUT] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    int valid = 1;
                    for (int i = 0; argv[optind][i] != '\0'; i++) {
                        if (!isdigit(argv[optind][i])) {
                            valid = 0;
                            break;
                        }
                    }
                    
                    if (valid == 1) {
                        data[OUT].amount = atoi(argv[optind]);
                    } else {
                        fprintf(stderr, "<amount> must integer\n");
                        exit(EXIT_FAILURE);
                    }
                    optind++;
                }
            } else {
                fprintf(stderr, "add argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc && argv[optind] != NULL) {
                if (argv[optind][0] == '-') {
                    flag[OUT] = 1;
                    otherFlag[1] = 1;
                    break;
                } else {
                    snprintf(data[OUT].desc, sizeof(data[OUT].desc), "%s", argv[optind]);
                    optind++;
                }
            } else {
                fprintf(stderr, "option -o need two argument: <amount> <description>\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc && argv[optind] != NULL && argv[optind][0] != '-') {
                otherFlag[4] = 1;
            }

            flag[OUT] = 1;
            otherFlag[1] = 1;
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

  // top priority options
    if (flag[HELP] == 1) {
        help(0);
        
        // puts("help");
        return 0;
    }

    if (flag[VER] == 1) {
        help(1);
        
        // puts("version");
        return 0;
    }    
    

  // handling options conflict
    if (otherFlag[0] == 1) {
        fprintf(stderr, "input an option or argument, see -h for help\n");
        exit(EXIT_FAILURE);
    }

    if (otherFlag[1] != 1) {
        fprintf(stderr, "argument not found, see -h for help\n");
        exit(EXIT_FAILURE);
    }
    
    if (flag[F] == 0 && (flag[LA] == 1 | flag[IN] == 1 | flag[OUT] == 1 | flag[NEW] == 1)) {
        fprintf(stderr, "option -f is required for this option ( -l, -n, -i, -o)\n");
        exit(EXIT_FAILURE);
    }

    if (flag[LA] == 1 && (flag[NEW] == 1 | flag[IN] == 1 | flag[OUT] == 1)) {
        fprintf(stderr, "option -l must standalone with -f\n");
        exit(EXIT_FAILURE);
    }

    if ((flag[NEW] == 1 && (flag[IN] == 1  ||  flag[OUT] == 1)) ||
        (flag[IN] == 1 && (flag[NEW] == 1  ||  flag[OUT] == 1)) ||
        (flag[OUT] == 1 && (flag[IN] == 1  || flag[NEW] == 1))) {
        fprintf(stderr, "choose one! ( -n, -i, -o )\n");
        exit(EXIT_FAILURE);
    }

    if (otherFlag[4] == 1) {
        fprintf(stderr, "write double quote (\" \") to add description with spaces\n");
        exit(EXIT_FAILURE);
    }
    

  // options logic           
    if (flag[F] == 1) {
        if (!filename) {
            fprintf(stderr, "filename is NULL\n");
            exit(EXIT_FAILURE);
        } else if (filename[0] == '-') {
            fprintf(stderr, "don't use dash '-' as the first character\n");
            exit(EXIT_FAILURE);
        }

        if (checkExistingFile(filename) != 0) {
            gSavings = getSavings(filename);
            getHeader(filename, gDate, gTitle);
            getCurrentTime(1, data[F].date);        
            if (strcmp(data[F].date, gDate) != 0) {
                flag[NEW] = 1;
                otherFlag[3] = 1;
            }
        }

        if (otherFlag[2] == 1) {
            fprintf(stderr, "add this option ( -l, -n, -i, -o ) to operate\n");
            exit(EXIT_FAILURE);
        }
        
        // puts("filename");
    }

    if (flag[LA] == 1) {
        int flag;
        if (getLastActivityValue(filename, &data[LA], &flag) != -1) {
            if (flag == 1) {
                printf("date: %s,\ntitle: %s,\nsavings: Rp %d\n", data[LA].date, data[LA].title, data[LA].savings);
                exit(EXIT_SUCCESS);
            } else if (flag == 2) {
                printf("clock: %s,\nactivity: %s cash,\namount: Rp %d\ndescription: %s\nsavings: Rp %d\n",
                       data[LA].clock, data[LA].activity, data[LA].amount, data[LA].desc, data[LA].savings);
                exit(EXIT_SUCCESS);
            }
        } else {
            fprintf(stderr, "file not found, see -h for help\n");
            exit(EXIT_FAILURE);
        }

        // puts("Last Activity");

    }

    if (flag[NEW] == 1) {
        getCurrentTime(1, data[NEW].date);

        if (checkExistingFile(filename) == -1) {
            logfile = fopen(filename, "a");
            if (!logfile) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(logfile, "\n");

        } else if (checkExistingFile(filename) == 0) {        
            logfile = fopen(filename, "w");
            if (!logfile) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
        }
        if (otherFlag[3] == 1) {
            fprintf(logfile, "%10s, %s, savings: Rp %d\n", data[NEW].date, gTitle, gSavings);
        } else {
            gSavings += data[NEW].savings;
            fprintf(logfile, "%10s, \"%s\", savings: Rp %d\n", data[NEW].date, data[NEW].title, gSavings);
        }
        fclose(logfile);

        // puts("new record");

    }

    if (flag[IN] == 1) {
        getCurrentTime(0, data[IN].clock);

        logfile = fopen(filename, "a");
        if (!logfile) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
            
        gSavings += data[IN].amount;
        fprintf(logfile, "%8s, incoming cash, amount: Rp %d, description: \"%s\", savings: Rp %d\n",
                data[IN].clock, data[IN].amount, data[IN].desc, gSavings);
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
            
        fprintf(logfile, "%8s, outgoing cash, amount: Rp %d, description: \"%s\", savings: Rp %d\n",
                data[OUT].clock, data[OUT].amount, data[OUT].desc, gSavings);
        fclose(logfile);

        // puts("outgoing cash");

    }
        
    return 0;
}
