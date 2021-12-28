#include <stdio.h>
#include <string.h>


typedef struct {
    char tag[3];
    char major;
    char ver;
    char flag;
    char tag_size[4];
} mainHeader;

typedef struct {
    char name[4];
    char size[4];
    char flags[2];
} frameHeader;

int len_number(char size[4]) {
    return ((size[0] << 21) | (size[1] << 14) | (size[2] << 7) | (size[3]));
}

int write_resize(char *value, FILE *bufFile) {
    int value_len = strlen(value);
    fputc((value_len + 1) / 4096, bufFile);
    fputc(((value_len + 1) % 4096) / 256, bufFile);
    fputc(((value_len + 1) % 4096 % 256) / 16, bufFile);
    fputc((value_len + 1) % 4096 % 256 % 16, bufFile);
    return value_len;
}


void show(char *adres) {
    mainHeader main_h;
    frameHeader frame_h;
    FILE *file = fopen(adres, "rb");
    fread(&main_h, 10, 1, file);
    int size_tag = len_number(main_h.tag_size);
    int i = 0;

    while (i < size_tag) {
        fread(&frame_h, 10, 1, file);
        if (frame_h.name[0] < 'A' || frame_h.name[0] > 'Z') {
            break;
        }
        int size_frame =
                frame_h.size[0] * 16 * 16 * 16 + frame_h.size[1] * 16 * 16 + frame_h.size[2] * 16 + frame_h.size[3];
        char inform_frame[size_frame];
        fread(&inform_frame, size_frame, 1, file);
        i += (10 + size_frame);

        printf("%s ", frame_h.name);
        for (int j = 0; j < size_frame; j++) {
            printf("%c", inform_frame[j]);

        }
        printf("\n");
    }
}

void get(char *adres, char *field) {
    mainHeader main_h;
    frameHeader frame_h;
    FILE *file = fopen(adres, "rb");
    fread(&main_h, 10, 1, file);
    int size_tag = len_number(main_h.tag_size);
    int i = 0;
    while (i < size_tag) {
        fread(&frame_h, 10, 1, file);
        if (frame_h.name[0] < 'A' || frame_h.name[0] > 'Z') {
            break;
        }
        int size_frame =
                frame_h.size[0] * 16 * 16 * 16 + frame_h.size[1] * 16 * 16 + frame_h.size[2] * 16 + frame_h.size[3];
        char inform_frame[size_frame];
        fread(&inform_frame, size_frame, 1, file);
        i += (10 + size_frame);
        if (strcmp(frame_h.name, field) == 0) {
            printf("%s", field);
            for (int j = 0; j < size_frame; j++) {
                printf("%c", inform_frame[j]);
            }
            break;
        }
    }
}


void set(char *adres, char *field, char *value) {
    mainHeader main_h;
    frameHeader frame_h;
    FILE *file = fopen(adres, "rb");
    FILE *bufFile = tmpfile();
    fread(&main_h, 10, 1, file);
    fwrite(main_h.tag, 1, 3, bufFile);
    fputc(main_h.major, bufFile);
    fputc(main_h.ver, bufFile);
    fputc(main_h.flag, bufFile);
    fwrite(main_h.tag_size, 1, 4, bufFile);
    int fl = 1;
    while (!feof(file)) {
        if (fl == 1) {
            fread(&frame_h, 10, 1, file);
            if (frame_h.name[0] < 'A' || frame_h.name[0] > 'Z') {
                fl = 0;
            }
        }
        if (strcmp(frame_h.name, field) != 0) {
            if (fl == 1) {
                fwrite(frame_h.name, 1, 4, bufFile);
                fwrite(frame_h.size, 1, 4, bufFile);
                fwrite(frame_h.flags, 1, 2, bufFile);
                int size_frame =
                        frame_h.size[0] * 16 * 16 * 16 + frame_h.size[1] * 16 * 16 + frame_h.size[2] * 16 +
                        frame_h.size[3];
                char inform_frame[size_frame];
                fread(&inform_frame, size_frame, 1, file);
                fwrite(inform_frame, 1, size_frame, bufFile);
            } else {
                fputc(fgetc(file), bufFile);
            }
        } else {
            fwrite(frame_h.name, 1, 4, bufFile);
            int value_len = write_resize(value, bufFile);
            fwrite(frame_h.flags, 1, 2, bufFile);
            fputc(0, bufFile);
            fwrite(value, 1, value_len, bufFile);
            fseek(file, (frame_h.size[0] * 16 * 16 * 16 + frame_h.size[1] * 16 * 16 + frame_h.size[2] * 16 +
                         frame_h.size[3]), SEEK_CUR);
        }
    }
    FILE *file_ren = fopen(adres, "wb");
    fseek(bufFile, 0, SEEK_SET);
    while (!feof(bufFile)) {
        fputc(fgetc(bufFile), file_ren);
    }
}

char value[30];

int main(int argc, char *argv[]) {
    char adres[strlen(argv[1]) - 11];
    for (int i = 0; i < strlen(argv[1]) - 11; i++) {
        adres[i] = argv[1][i + 11];
    }
    if (strcmp(argv[2], "--show") == 0) {
        show(adres);
    } else {
        char frame[5];
        frame[0] = argv[2][6];
        frame[1] = argv[2][7];
        frame[2] = argv[2][8];
        frame[3] = argv[2][9];
        if (argv[2][2] == 'g') {
            get(adres, frame);
        } else if (argv[2][2] == 's') {
            for (int i = 0; i < strlen(argv[3]) - 8; i++) {
                value[i] = argv[3][i + 8];
            }
            set(adres, frame, value);
        }
    }
}

