#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <stdint.h>

#pragma pack(push, 1)

/*fseek(file, 0, SEEK_SET);
   FILE *file2 = fopen("test.bmp", "wb");
    while (!feof(file)) {
        fputc(fgetc(file), file2);
    }*/

typedef struct {
    unsigned char bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    uint32_t biWidht;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImages;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPFILEINFO;
#pragma pack(pop)

void gameinLife(unsigned char **img, int height, int widht) {
    short exitFlag = 0;
    unsigned char **temp = (unsigned char **) malloc(height * sizeof(unsigned char *));
    for (int y = 0; y < height; y++) {
        temp[y] = (unsigned char *) malloc(widht * sizeof(unsigned char));
    }
    int num = 0;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < widht - 1; x++) {
            num = img[y + 1][x - 1] + img[y + 1][x] + img[y + 1][x + 1] + img[y][x - 1] + img[y][x + 1] +
                  img[y - 1][x - 1] + img[y - 1][x] + img[y - 1][x + 1];
            if (img[y][x] == 1) {
                if (num < 2 || num > 3) {
                    exitFlag = 1;
                    temp[y][x] = 0;
                } else {
                    temp[y][x] = 1;
                }
            } else {
                if (num == 3) {
                    exitFlag = 1;
                    temp[y][x] = 1;
                } else {
                    temp[y][x] = 0;
                }
            }
        }
    }
    if (exitFlag != 1) {
        exit(0);
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < widht; x++) {
            if (img[y][x] == 1) {
                exitFlag = 2;
            }
            img[y][x] = temp[y][x];
        }
    }
    if (exitFlag != 2) {
        exit(0);
    }


    for (int i = 0; i < height; i++) {
        free(temp[i]);
    }

    free(temp);
}

void byteintoBit(unsigned char byte, unsigned char bites[8]) {
    unsigned char del = 128;
    for (int i = 0; i < 8; i++) {
        if (byte >= del) {
            bites[i] = 1;
            byte -= del;
        } else {
            bites[i] = 0;
        }
        del /= 2;
    }
}

unsigned char bitintoByte(const unsigned char bites[8]) {
    unsigned char mult = 128;
    unsigned char res = 0;
    for (int i = 0; i < 8; i++) {
        res += bites[i] * mult;
        mult /= 2;
    }
    return res;
}

void intoDoubleArray(unsigned char **img, unsigned char *imageinByte, int height, int widht) {
    unsigned char bites[8];
    int x = 0, y = 0;
    for (int i = 0; i < height * widht / 8; i++) {
        byteintoBit(imageinByte[i], bites);
        for (int j = x; j < x + 8; j++) {
            img[y][j] = bites[j - x];
        }
        if ((x) >= widht - 8) {
            x = 0;
            y += 1;
        } else {
            x += 8;
        }
    }
}

void intoArray(unsigned char **img, unsigned char *imageinByte, int height, int widht) {
    int k = 0;
    unsigned char bites[8];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < widht; x += 8) {
            for (int j = x; j < x + 8; j++) {
                bites[j - x] = img[y][j];
            }
            imageinByte[k] = bitintoByte(bites);
            k++;
        }
    }
}

int InpOut(FILE *startImage, FILE *res) {
    BITMAPFILEHEADER mapheader;
    BITMAPFILEINFO mapinfo;
    fread(&mapheader, sizeof(mapheader), 1, startImage);
    fread(&mapinfo, sizeof(mapinfo), 1, startImage);
    if (mapinfo.biWidht % 8 != 0) {
        mapinfo.biWidht += 8 - (mapinfo.biWidht % 8);
    }
    unsigned char *imageinByte = (unsigned char *) malloc(mapinfo.biWidht * mapinfo.biHeight / 8);
    unsigned char color[mapheader.bfOffBits - 54];
    unsigned char ost[mapheader.bfSize - mapheader.bfOffBits - (mapinfo.biWidht * mapinfo.biHeight / 8)];

    fread(&color, 1, mapheader.bfOffBits - 54, startImage);
    fread(imageinByte, 1, mapinfo.biWidht * mapinfo.biHeight / 8, startImage);
    fread(&ost, 1, mapheader.bfSize - mapheader.bfOffBits - (mapinfo.biWidht * mapinfo.biHeight / 8), startImage);

    unsigned char **img = (unsigned char **) malloc(mapinfo.biHeight * sizeof(unsigned char *));
    for (int y = 0; y < mapinfo.biHeight; y++) {
        img[y] = (unsigned char *) malloc(mapinfo.biWidht * sizeof(unsigned char));
    }


    intoDoubleArray(img, imageinByte, mapinfo.biHeight, mapinfo.biWidht);
    gameinLife(img, mapinfo.biHeight, mapinfo.biWidht);
    intoArray(img, imageinByte, mapinfo.biHeight, mapinfo.biWidht);


    fwrite(&mapheader, sizeof(mapheader), 1, res);
    fwrite(&mapinfo, sizeof(mapinfo), 1, res);
    fwrite(&color, 1, mapheader.bfOffBits - 54, res);

    fwrite(imageinByte, 1, mapinfo.biWidht * mapinfo.biHeight / 8, res);
    fwrite(&ost, 1, mapheader.bfSize - mapheader.bfOffBits - (mapinfo.biWidht * mapinfo.biHeight / 8), res);

    for (int i = 0; i < mapinfo.biHeight; i++) {
        free(img[i]);
    }
    free(img);
    free(imageinByte);
}

int main(int argc, char *argv[]) {
    FILE *startImage;
    FILE *res;
    int num = 1;
    char *dir_name;
    char way[20];
    char filename[10];
    int max_iter = INT_MAX;
    int dumpfreq = 1;
    for (int i = 0; i < argc; i++) {
        if (!strcmp("--input", argv[i])) {
            startImage = fopen(argv[i + 1], "rb");
        }
        if (!strcmp("--output", argv[i])) {
            dir_name = argv[i + 1];
            mkdir(dir_name);
        }
        if (!strcmp("--max_iter", argv[i])) {
            max_iter = strtol(argv[i + 1], 0, 10);
        }
        if (!strcmp("--dump_freq", argv[i])) {
            dumpfreq = strtol(argv[i + 1], 0, 10);
        }
    }
    if (max_iter<=0 || dumpfreq <=0){
        printf("ERROR, incorrect input");
        exit(0);
    }
    for (int i = 0; i < max_iter; i++) {

        strcpy(way, dir_name);
        strcat(strcat(way, "\\"), strcat(itoa(num, filename, 10), ".bmp"));
        res = fopen(way, "wb");
        num += 1;
        InpOut(startImage, res);
        fclose(startImage);
        fclose(res);
        startImage = fopen(way, "rb");
        memset(way, 0, sizeof(way));
    }
    for (int i=1; i<=num-1; i++){
        if (i % dumpfreq != 0){
            strcpy(way, dir_name);
            strcat(strcat(way, "\\"), strcat(itoa(i, filename, 10), ".bmp"));
            remove(way);
        }
    }
}

//app.exe --input picture.bmp --output images --max_iter 10
//app.exe --input picture.bmp --output image --max_iter 15 --dump_freq 5