#include <stdio.h>
#include <string.h>

void create(char *arcName, int argc, char *argv[]) {
    FILE *arcfile = fopen(arcName, "wb");
    FILE *file;
    for (int i = 4; i < argc; i++) {
        file = fopen(argv[i], "rb");
        if (file == NULL) {
            continue;
        }
        fseek(file, 0, SEEK_END);
        fprintf(arcfile, "< %s - %llu >", argv[i], ftell(file));
        fseek(file, 0, SEEK_SET);
        fclose(file);
    }
    fprintf(arcfile, "\n");
    for (int i = 4; i < argc; i++) {
        file = fopen(argv[i], "rb");
        if (file == NULL) {
            printf("%s  is not founded\n", argv[i]);
            continue;
        }
        int c;
        while ((c = getc(file)) != EOF){
            putc(c, arcfile);
        }
        fclose(file);
        printf("Into %s added %s\n", arcName, argv[i]);
        remove(argv[i]);
    }
}

void extract(char *arcName) {
    FILE *arcfile = fopen(arcName, "rb+wb");
    FILE *file;
    char fname[50];
    unsigned long long int begData = 0;
    int c;
    while ((c = getc(arcfile)) != EOF) {
        begData++;
        if (c == '\n') {
            break;
        }
    }
    fseek(arcfile, 0, SEEK_SET);
    printf("from %s input:\n", arcName);
    unsigned long long int fsize;
    while (fscanf(arcfile, "< %s - %llu >", fname, &fsize) != 0) {
        file = fopen(fname, "wb");
        unsigned long long int currPos = ftell(arcfile);
        fseek(arcfile, begData, SEEK_SET);
        begData += fsize;
        while ((fsize--) > 0) {
            putc(getc(arcfile), file);
        }
        fseek(arcfile, currPos, SEEK_SET);
        fclose(file);
        printf(" %s\n", fname);
    }
}

void list(char *arcName) {
    FILE *arcfile = fopen(arcName, "rb");
    char fname[50];
    unsigned long long fsize;
    printf("File list:\n");
    while (fscanf(arcfile, "< %s - %llu >", fname, &fsize) != 0) {
        printf(" %s\n", fname);
    }
    fclose(arcfile);
}

int main(int argc, char *argv[]) {
    int i;
    char *arcName;
    for (i = 0; i < argc; i++) {
        if (!strcmp("--file", argv[i])) {
            arcName = argv[i + 1];
        }
        if (!strcmp("--create", argv[i])) {
            create(arcName, argc, argv);
            break;
        }
        if (!strcmp("--extract", argv[i])) {
            extract(arcName);
        }
        if(!strcmp("--list", argv[i])){
            list(arcName);
        }
    }
}

//app.exe --file data.arc --create one.bmp two.bmp three.txt
//app.exe --file data.arc --list
//app.exe --file data.arc --extract