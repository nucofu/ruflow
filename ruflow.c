#include "ruflow.h"

// about

void help(int ver) {

    char desc[] = "ruflow/rupiah flow - simple cashflow manager for managing your own personal cashflow record\n";
    
    char usage[] = "  ruflow [options]\n";
    char options[] = "  -h                             show help message\n  -v                             show versions\n  -f <file>                      creating or accessing file\n    -l                             show last activity\n    -n <saving> <write>            title new record\n    -i <amount> <description>      write incoming cash\n    -o <amount> <description>      write outgoing cash\n";
    char examples[] = "  ruflow -f cashflow.log -n 200000 \"Ucok Cashflow\"\n  ruflow -f cashflow.log -i 5000 \"from udin\"\n  ruflow -f cashflow.log -o 2000 \"from icih\"\n";
    char versions[] = "ruflow/rupiah flow - version protoype 3.1\n";

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
        if (sscanf(buff, "%10[^,], %31[^,], savings: Rp%d\n", temp.date, temp.title, &temp.savings) == 3)
            
            {
                *new0 = temp;
                *flag = 1;
                continue;
            }
 
        if (sscanf(buff, "%8[^,], %8s cash, amount: Rp%d, description: %127[^,], savings: Rp%d\n",
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

// main function

int main(int argc, char *argv[]) {

    int mainFlag[3];
    mainFlag[0] = 0; // error flag if ruflow have no argument
    mainFlag[1] = 0; // error flag if argument not found
    mainFlag[2] = 0; // warning flag if only inserting -f option
    
    if (argc == 1) {
        mainFlag[0] = 1;
    }

    // global options variable
    FILE *logfile;
    valyu data[4];
    char *filename;

    int gSavings;
    gSavings = 0;
    
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
    while ((opt = getopt(argc, argv, "hvf:ln:i:o:")) != -1) {
        switch(opt) {

        case 'h': {
            flag[HELP] = 1;
            mainFlag[1] = 1;
            break;
        }

        case 'v': {
            flag[VER] = 1;
            mainFlag[1] = 1;
            break;
        }
            
        case 'f': {
            if (optarg != NULL) {
                filename = strdup(optarg);
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }
            
            flag[F] = 1;
            mainFlag[1] = 1;

            if (argv[optind] == NULL) {
                mainFlag[2] = 1;
            }
            
            break;
        }
                        
        case 'l': {
            flag[LA] = 1;
            mainFlag[1] = 1;
            break;
        }
            
        case 'n': {
            if (optarg != NULL) {
                int valid = 1;
                for (int i = 0; optarg[i] != '\0'; i++) {
                    if (!isdigit(optarg[i])) {
                        valid = 0;
                        break;
                    }
                }
                
                if (valid == 1) {
                    data[NEW].savings = atoi(optarg);
                } else {
                    fprintf(stderr, "<saving> must integer\n");
                    exit(EXIT_FAILURE);
                }
                
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }
            
            if (optind < argc) {
                snprintf(data[NEW].title, sizeof(data[NEW].title), "%s", argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -n need two argument: <saving> <title>\n");
                exit(EXIT_FAILURE);
            }

            flag[NEW] = 1;
            mainFlag[1] = 1;
            break;
        }
            
        case 'i': {
            if (optarg != NULL) {                
                int valid = 1;
                for (int i = 0; optarg[i] != '\0'; i++) {
                    if (!isdigit(optarg[i])) {
                        valid = 0;
                        break;
                    }
                }
                
                if (valid == 1) {
                    data[IN].amount = atoi(optarg);
                } else {
                    fprintf(stderr, "<amount> must integer\n");
                    exit(EXIT_FAILURE);
                }
                
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc) {
                snprintf(data[IN].desc, sizeof(data[IN].desc), "%s", argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -i need two argument: <amount> <description>\n");
                exit(EXIT_FAILURE);
            }
            
            flag[IN] = 1;
            mainFlag[1] = 1;
            break;
        }
            
        case 'o': {
            if (optarg != NULL) {
                int valid = 1;
                for (int i = 0; optarg[i] != '\0'; i++) {
                    if (!isdigit(optarg[i])) {
                        valid = 0;
                        break;
                    }
                }
                
                if (valid == 1) {
                    data[OUT].amount = atoi(optarg);
                } else {
                    fprintf(stderr, "<amount> must integer\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "invalid argument\n");
                exit(EXIT_FAILURE);
            }

            if (optind < argc) {
                snprintf(data[OUT].desc, sizeof(data[OUT].desc), "%s", argv[optind]);
                optind++;
            } else {
                fprintf(stderr, "option -o need two argument: <amount> <description>\n");
                exit(EXIT_FAILURE);
            }

            flag[OUT] = 1;
            mainFlag[1] = 1;
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
    if (mainFlag[0] == 1) {
        fprintf(stderr, "input an option or argument, see -h for help\n");
        exit(EXIT_FAILURE);
    }

    if (mainFlag[1] != 1) {
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
        }

        if (mainFlag[2] == 1) {
            printf("add this option ( -l, -n, -i, -o ) to operate\n");
        }
        
        // puts("filename");
    }

    if (flag[LA] == 1) {
        int flag;
        if (getLastActivityValue(filename, &data[LA], &flag) != -1) {
            if (flag == 1) {
                printf("date: %s,\ntitle: %s,\namount: %d\n", data[LA].date, data[LA].title, data[LA].amount);
            } else if (flag == 2) {
                printf("clock: %s,\nactivity: %s cash,\namount: Rp%d\ndescription: %s\namount: Rp%d\n",
                       data[LA].clock, data[LA].activity, data[LA].amount, data[LA].desc, data[LA].savings);
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
        gSavings += data[NEW].savings;
        fprintf(logfile, "%10s, %s, savings: Rp%d\n", data[NEW].date, data[NEW].title, gSavings);
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
        fprintf(logfile, "%8s, incoming cash, amount: Rp%d, description: %s, savings: Rp%d\n",
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
            
        fprintf(logfile, "%8s, outgoing cash, amount: Rp%d, description: %s, savings: Rp%d\n",
                data[OUT].clock, data[OUT].amount, data[OUT].desc, gSavings);
        fclose(logfile);

        // puts("outgoing cash");

    }
        
    return 0;
}
