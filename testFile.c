#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

void cpuinfo();

int main(int argc, char **argv) {
    cpuinfo();
    return 0;
}

void cpuinfo() {
    FILE *file;
    int text = 0;
    file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Virtual memory error opening the file");
        exit(EXIT_FAILURE);
    }
    while((text = fgetc(file)) != EOF) {
	printf("%c", text);
	
    }
    fclose(file);
}


