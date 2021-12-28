#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void str(char *adres) {
    FILE *f = fopen(adres, "r");
    if (f == NULL) {
        printf("Error");
        exit(0);
    }
    int sr = 1;
    char c;
    c = fgetc(f);
    while (c != EOF) {
        if (c == '\n') sr++;
        c = fgetc(f);
    }
    printf("lines: %d \n", sr);
    fclose(f);
}

void byte(char *adres) {
    FILE *f = fopen(adres, "r");
    if (f == NULL) {
        printf("Error");
        exit(0);
    }
    int b = 0;
    char c;
    c = fgetc(f);
    while (c != EOF) {
        b++;
        if (c == '\n') b++;
        c = fgetc(f);
    }
    printf("bytes: %d\n", b);
    fclose(f);
}

void wrd(char *adres) {

    FILE *f = fopen(adres, "r");
    if (f == NULL) {
        printf("Error");
        exit(0);
    }
    int w = 0;
    int c = fgetc(f);
    while (c != EOF) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            while (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
                c = fgetc(f);
            }
            w++;
        } else {
            c = fgetc(f);
        }
    }
    printf("words: %d \n", w);

}

int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
        str(argv[2]);
    }
    if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
        byte(argv[2]);
    }
    if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
        wrd(argv[2]);
    }
}

