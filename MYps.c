#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX_SIZE 100


struct proc {
    int pid;
    char state; //-s
    char command[200];
    unsigned int utime;
    unsigned int stime; //-S
    unsigned int vMemory;
};

bool containsProcessInformation(char *argument);
char *commandline(char *PID);
char stateInformation(char *PID);
int userTime(char *PID);
int systemTime(char *PID);
int virtualMemory(char *PID);
void printElementsOfStruct(struct proc myProc);
char **getSameUserID();
char *printProcessInfo(char *PID);
struct proc getAllTheInformation(char* PIDpath, int argc, char **argv);
struct proc informationOfSameUserID(char *PID, int argc, char **argv);
//bool valueAfterPIsAnInteger(char *argument);

/*

The main function is taking arguments from the user
If the argument contains -p, then I am saving the element after it and saving it in PIDpath
If contains -p I am going inside getAllTheInformation() which returns a proc based on argument given.
If the arguments does not contain -p I am calling the getSameUserID()
This function is returning a pointer to a pointer, aka a 2D array. 
This array contains all the same PID.
Then I am looping through the array and sending each of the PID into informationOfSameUserID
Which returns a proc and then I saving that into a proc.
Lastly, I am calling the printElementsOfProc(), which takes proc as a parameter.

*/ 

int main(int argc, char **argv) {
    int locationOfPID = 0;
    char PIDpath[4097];
    bool flag = false;
  
    for (int i = 1; i < argc; i++) {
        //printf("Argument %d = %s\n", i, *(argv+i));
        if (containsProcessInformation(argv[i])) {
            locationOfPID = i+1;
            flag = true;
            break;
        }
    } 
    if (flag) {
        strcpy(PIDpath, argv[locationOfPID]);
        struct proc someProc = getAllTheInformation(PIDpath, argc, argv);
        printElementsOfStruct(someProc);
    } else {
        //printf("%s\n", "Entering in else");
        char** result = getSameUserID();
        for (int i = 0; result[i] != NULL; i++) {
            //printf("Here are the same user ID %s\n", result[i]);
            struct proc myProc = informationOfSameUserID(result[i], argc, argv);
            printElementsOfStruct(myProc);
        }
    }
    return 0;
}

/*
This is the function I am calling if (argv) contains -p. 
The function return a struct proc. The proc is saving all the information based on the argv. 
For example, if there is an argument, I am calling the function accordingly
If argument contains:
-s then stateInformation
-S then systemTime
-v then virtual memory
If the arguments (argv) does not contain one of the elements I am setting the value 0 or "\0"
accordingly. 
In the end I am returning the proc and then I am printingAllTheElementsOfProc

*/

struct proc getAllTheInformation(char* PIDpath, int argc, char **argv) {
    int containsUtime = 0;
    int containsCommandLine = 0;
    int option;
    struct proc myProc;
    myProc.pid = atoi(PIDpath);

    bool si = false;
    bool st = false;
    bool vm = false;

    while ((option = getopt(argc, argv, "sSUvcp")) != -1) {
        //printf("%s %c\n", "option = ", option);
        switch(option) {
            case 's':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                char result = stateInformation(PIDpath);
                //printf("result -s: %c\n", result);
                myProc.state = result;
                si = true;
                break;
            case 'S':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                int resultS = systemTime(PIDpath);
                //printf("result -S: %d\n", resultS);
                myProc.stime = resultS;
                st = true;
                break;
            case 'v':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                int resultV = virtualMemory(PIDpath);
                //printf("result -v: %d\n", resultV);
                myProc.vMemory = resultV;
                vm = true;
                break;
            case 'U':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                printf("%s\n", "Entering U time");
                containsUtime = 1;
                break;
            case 'c':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                containsCommandLine = 1;
                break;
            case 'p':
                continue;
            default:
                fprintf(stderr, "Usage: %s [-p PID] [-s] [-S] [-v] [-U] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!si) {
        myProc.state = '\0';
    }
    if (!st) {
        myProc.stime = -1;
    }
    if (!vm) {
        myProc.vMemory = -1;
    }

    if (!containsUtime) {
        //printf("%s\n", "Entering U time part 2");
        int result = userTime(PIDpath);
        myProc.utime = result;
    } else {
        myProc.utime = -1;
    }

    if (!containsCommandLine) {
        char *result = commandline(PIDpath);
        strncpy(myProc.command, result, sizeof(myProc.command));
        free(result);
    }
    return myProc;
}

/*
Information of same user ID take a char *PID, number of arguments, and all the argv arguments
from the user.
If the argc is 1, that means only MYps is the argument. If that is the case, I am printing 
the commandLine and the userTime. The rest of the value in the proc are being set to -1 or "\0".
Else if there is some more argument, I am calling the function accordingly
If argument contains:
-s then stateInformation
-S then systemTime
-v then virtual memory
If the arguments (argv) does not contain one of the elements I am setting the value 0 or "\0"
accordingly. 
Lastly, I am returning a proc. 

*/

struct proc informationOfSameUserID(char *PID, int argc, char **argv) {
    //printf("%s\n", "Entering teh function");
    struct proc myProc;
    myProc.pid = atoi(PID);
    if (argc == 1) {
        //prints cmdline, utime, and PID of each process with your UID
        char *result = commandline(PID);
        strncpy(myProc.command, result, sizeof(myProc.command));
        free(result);
        int x = userTime(PID);
        if (x >= 0) {
            myProc.utime = x;
        }
        myProc.state = '\0';
        myProc.stime = -1;
        myProc.vMemory = -1;

    } else {
        int containsUtime = 0;
        int containsCommandLine = 0;

        bool si = false;
        bool st = false;
        bool vm = false;
        
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-s") == 0) {
                char result = stateInformation(PID);
                myProc.state = result;
                si = true;
            } else if (strcmp(argv[i], "-S") == 0) {
                int resultS = systemTime(PID);
                myProc.stime = resultS;
                st = true;
            } else if (strcmp(argv[i], "-v") == 0) {
                int resultV = virtualMemory(PID);
                myProc.vMemory = resultV;
                vm = true;
            } else if (strcmp(argv[i], "-U") == 0) {
                containsUtime = 1;
            } else if (strcmp(argv[i], "-c") == 0) {
                containsCommandLine = 1;
            } else {
                fprintf(stderr, "Usage: %s [-p PID] [-s] [-S] [-v] [-U] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }

        if (!si) {
            myProc.state = '\0';
        }
        if (!st) {
            myProc.stime = -1;
        }
        if (!vm) {
            myProc.vMemory = -1;
        }
               
        if (!containsUtime) {
            int result = userTime(PID);
            myProc.utime = result;
        } else {
            myProc.utime = -1;
        }

        if (!containsCommandLine) {
            char *result = commandline(PID);
            strncpy(myProc.command, result, sizeof(myProc.command));
            free(result);
        }
    }    
    return myProc;
}

/*
This process checks if the argument provided is -p. Using it in the top to loop through all argv.
If contains then returns true, else false.
*/

bool containsProcessInformation(char *argument) {
    if (strcmp(argument, "-p") == 0) {
        return true;
    } 
    return false;
}



/*
the stateInformation contains PID as the argument. 
I am opening the file and then looping through the file
Looking at the manual of stat, I saw that the third paramater is the state information.
Furthermore, I am saving that char in processsState, and then returning stateInformation. 


*/

char stateInformation(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    char processState = '\0';
    //char toReturn;

    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Stat Information error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
        //printf("text = %c\n", text);
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 3) {
            processState = (char)text;
        }
    }
    fclose(file);
    return processState;
}

/*
the userTime contains PID as the argument. 
I am opening the file and then looping through the file
Looking at the manual of stat, I saw that the 14th paramater is the user time information.
Furthermore, I am saving that int in result, and then returning result.



*/

int userTime(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    //printf("%s\n", pathName);
    if (file == NULL) {
        perror("User time Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 14) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

/*
the systemTime contains PID as the argument. 
I am opening the file and then looping through the file
Looking at the manual of stat, I saw that the 15th paramater is the system time information.
Furthermore, I am saving that int in result, and then returning result.
 
*/

int systemTime(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    //printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("System time Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 15) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

/*

int virtualMemory returns the virtual memory of the PID. I am open the file based on the path
From there I am 
*/

int virtualMemory(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/statm", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Virtual memory error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    //printf("%s\n", "statm");
    while ((text = fgetc(file)) != EOF) {
         if (text == ' ') {
            count++;
            continue;
        }
        if(count == 1) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

/*
commandLine takes PID as an argument. 
Next I am opening the file and then looping though the contents of it. 
Next I am reallocating memory by size + 2
The result is being saved in a temporary pointer 
Then toReturn point to temp which is the newly allocated memory
And then save the charcater read from toReturn at size
And then I finally increase the size. 

*/
char *commandline(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int size = 0;
    char *toReturn = NULL;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/cmdline", PID);
    //printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Command line Error opening the file");
        exit(EXIT_FAILURE);
    }
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
        char *temp = (char *)realloc(toReturn, size + 2);
        if (temp == NULL) {
            perror("Error reallocating memory");
            free(toReturn);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        toReturn = temp;
        toReturn[size] = (char)text;
        size++;
    }
    toReturn[size] = '\0';
    fclose(file);
    return toReturn;
}

/*
This is probably the most important function. 
It prints the values of the proc. 


*/

void printElementsOfStruct(struct proc myProc) {
    printf("%s", "Output: ");
    if (myProc.pid != -1) {
        printf("%d: ", myProc.pid);
    }
    if (myProc.state != '\0') { 
        printf("%c ", myProc.state);
    }
    if (myProc.utime != -1) { 
        printf("utime=%d ", myProc.utime);
    }
    if (myProc.stime != -1) {
        printf("stime=%d ", myProc.stime);
    }
    if (myProc.vMemory != -1) {
        printf("vMemory=%d ", myProc.vMemory);
    }
    if (myProc.command[0] != '\0') {
        printf("%s\n", myProc.command);
    }
    printf("\n");
}

// bool valueAfterPIsAnInteger(char *argument) {
//     char *ptrAtTheEnd; 
//     int value = strtol(argument, &ptrAtTheEnd, 10);
//     if (*ptrAtTheEnd == "\0") {
//         return true;
//     }
// }

/*
The printProcessInfo finds same user ID as mine
First I open the file and then I am looping through the file. 
I am saving the entire line and then using sscanf
The usage of sscanf allows me to save what is after the "UID" which is the user id
Next, I compare the userID with mine.
If it is a match, then I am returning the PID;
If not then return Null.

*/

char *printProcessInfo(char *PID) {

    FILE *file;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", PID);
    file = fopen(path, "r");
    if (file == NULL) {
        printf("path = %s, PID = %s\n", path, PID);
        perror("Error in the path");
        exit(EXIT_FAILURE);
    }
    char entireLine[256];
    char uid[256];
    uid_t myUID = getuid();
    //printf("MY UID = %d\n", myUID);
    while (fgets(entireLine, sizeof(entireLine), file)) {
        if (sscanf(entireLine, "Uid: %s", uid) == 1) {
            if (atoi(uid) == myUID) {
                fclose(file);
                return PID;
            }
        }
    }
    fclose(file);
    return NULL;
}

/*
The getSameUserID function returns a pointer to the first element of the char *
I am opening the /proc directory. 
Next, I am rading from the directory and then I am saving each entry in entryName
Next I am checking if the entryName is a directory
And then I am sending each directory to printProcessInfo which return PID is the UID is same.
And then I am saving it in toReturn.
Finally closing directory and then retuning toReturn.

*/

char **getSameUserID() { //returns to a pointer to pointer to char. Or points to the char*;
    DIR *dirp;
    struct dirent *entryName;
    char **toReturn = (char**)malloc(MAX_SIZE * sizeof(char*));
    int i = 0;
    dirp = opendir("/proc");
    if (dirp == NULL) {
        perror("Error opening the directory");
        exit(EXIT_FAILURE);
    }
    while ((entryName = readdir(dirp)) != NULL) {
        if (entryName->d_type == DT_DIR && atoi(entryName->d_name) > 0) {
            // printf("Directory name = %s\n", entryName->d_name);
            char *PID = printProcessInfo(entryName->d_name);
            //printf("Returned PID is = %s\n", PID);
            if (PID != NULL) {
                toReturn[i] = PID;
                i++;
            }
        }
    }
    closedir(dirp);
    toReturn[i] = NULL;
    return toReturn;
}

