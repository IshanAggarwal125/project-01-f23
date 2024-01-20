#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>


void cpuinfo();
void memoryInfo();

int main(int argc, char **argv) {
    cpuinfo();
    memoryInfo();
    return 0;
}

void cpuinfo() {
    FILE *file;
    char entireLine[256];
    char cacheSize[256];
    int numberOfTheProcessor = -1;
    //int text = 0;
    file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Virtual memory error opening the file");
        exit(EXIT_FAILURE);
    }
    char *token;
    char *rest;
    //char *ptr;
    while (fgets(entireLine, sizeof(entireLine), file)) {
        rest = entireLine;
         while ((token = strtok_r(rest, ":\n", &rest))) { // tokenize the string by splitting. 
            char *trimmedToken = strtok(token, " \t"); // rest is the pointer to the string I want to tokenize. 
            if (trimmedToken == NULL) {
                continue;
            }
            if (strcmp(trimmedToken, "processor") == 0) {
                token = strtok_r(NULL, ":", &rest);
                if (token != NULL) {
                    numberOfTheProcessor = atoi(token); // getting the value which is next to processor.

                }
            } else if (strcmp(trimmedToken, "cache") == 0) {
                token = strtok_r(NULL, ":", &rest);
                if (token != NULL) {
                    strcpy(cacheSize, token);
                }
            } else {
                continue;
            }
            if (numberOfTheProcessor >= 0 && strlen(cacheSize) > 0) {
                printf("Processor %d with a cache size of KB %s\n", numberOfTheProcessor, cacheSize);
                numberOfTheProcessor = -1;
                cacheSize[0] = '\0';
            }
        }
    }      
    fclose(file);
}

void memoryInfo() {
    FILE *file;
    char entireLine[256];
    char *rest;
    char *token;
    char freeMemory[200];
    char buffer[200];
    file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Virtual memory error opening the file");
        exit(EXIT_FAILURE);
    }
    while (fgets(entireLine, sizeof(entireLine), file)) {
        rest = entireLine;
        while ((token = strtok_r(rest, ":\n", &rest))) {
            char *trimmedToken = strtok(token, " \t");
            if (trimmedToken == NULL) {
                continue;
            }
            if (strcmp(trimmedToken, "MemFree") == 0) {
                token = strtok_r(NULL, ":", &rest);
                if (token != NULL) {
                    strcpy(freeMemory, token);
                }
            } else if (strcmp(trimmedToken, "Buffers") == 0) {
                token = strtok_r(NULL, ":", &rest);
                if (token != NULL) {
                    strcpy(buffer, token);
                }
            } else {
                continue;
            }
        }
    }
    printf("Free memory %s\n", freeMemory);
    printf("Buffers: %s\n", buffer);
    fclose(file);
}


