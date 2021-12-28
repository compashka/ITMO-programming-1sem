#include <stdio.h>

int division(int *size_input, char *input) {
    int pointer_input = 0;
    int pointer_otvet = 0;
    unsigned char num[4];
    int remainder, a;
    if (*size_input == 1) {
        remainder = input[pointer_input++] - 48;
        pointer_otvet = 0;
        input[0] = '0';
    } else if (*size_input == 2) {
        remainder = (input[pointer_input++] - 48) * 10 + (input[pointer_input] - 48);
        pointer_otvet = 0;
        input[0] = '0';
    } else {
        num[0] = input[pointer_input++] - 48;
        num[1] = input[pointer_input++] - 48;
        num[2] = input[pointer_input++] - 48;

        if (num[0] * 100 + num[1] * 10 + num[2] < 256 && pointer_input < *size_input) {
            num[3] = input[pointer_input++] - 48;
        } else {
            num[3] = 255;
        }

        while (pointer_input <= *size_input) {
            if (num[2] != 255 && num[3] != 255) {
                a = (num[0] * 1000 + num[1] * 100 + num[2] * 10 + num[3]);
            } else if (num[3] == 255) {
                a = (num[0] * 100 + num[1] * 10 + num[2]);
            }
            input[pointer_otvet++] = a / 256 + 48;
            remainder = a % 256;
            if (remainder > 99) {
                if (pointer_input >= *size_input) {
                    break;
                }
                num[0] = remainder / 100;
                num[1] = (remainder / 10) % 10;
                num[2] = remainder % 10;
                num[3] = input[pointer_input++] - 48;

            } else if (remainder > 9) {
                if (pointer_input >= *size_input) {
                    break;
                }
                num[0] = remainder / 10;
                num[1] = remainder % 10;
                num[2] = input[pointer_input++] - 48;
                if (num[0] * 100 + num[1] * 10 + num[2] < 256) {
                    if (pointer_input + 1 <= *size_input) {
                        input[pointer_otvet++] = 0 + 48;
                        num[3] = input[pointer_input++] - 48;
                    } else {
                        remainder = num[0] * 100 + num[1] * 10 + num[2];
                        input[pointer_otvet++] = 0 + 48;
                        break;
                    }
                } else {
                    num[3] = 255;
                }
            } else if (remainder != 0) {
                num[0] = remainder % 10;
                if (pointer_input >= *size_input) {
                    break;
                }
                num[1] = input[pointer_input++] - 48;
                input[pointer_otvet++] = 0 + 48;
                if (pointer_input + 1 <= *size_input) {
                    num[2] = input[pointer_input++] - 48;
                } else {
                    remainder = num[0] * 10 + num[1];
                    break;
                }

                if (num[0] * 100 + num[1] * 10 + num[2] < 256) {
                    if (pointer_input + 1 <= *size_input) {
                        input[pointer_otvet++] = 0 + 48;
                        num[3] = input[pointer_input++] - 48;
                    } else {
                        remainder = num[0] * 100 + num[1] * 10 + num[2];
                        input[pointer_otvet++] = 0 + 48;
                        break;
                    }
                } else {
                    num[3] = 255;
                }
            } else {
                while (pointer_input < *size_input && input[pointer_input] - 48 == 0) {
                    input[pointer_otvet++] = 0 + 48;
                    pointer_input++;
                }
                if (pointer_input >= *size_input) {
                    break;
                }
                num[0] = input[pointer_input++] - 48;
                input[pointer_otvet++] = 0 + 48;
                if (pointer_input + 1 <= *size_input) {
                    input[pointer_otvet++] = 0 + 48;
                    num[1] = input[pointer_input++] - 48;
                } else {
                    remainder = num[0];
                    break;
                }
                if (pointer_input + 1 <= *size_input) {
                    num[2] = input[pointer_input++] - 48;
                } else {
                    remainder = num[0] * 10 + num[1];
                    break;
                }
                if (num[0] * 100 + num[1] * 10 + num[2] < 256 && (num[0] + num[1] + num[2] != 0)) {
                    if (pointer_input + 1 <= *size_input) {
                        input[pointer_otvet++] = 0 + 48;
                        num[3] = input[pointer_input++] - 48;
                    } else {
                        remainder = num[0] * 100 + num[1] * 10 + num[2];
                        input[pointer_otvet++] = 0 + 48;
                        break;
                    }
                } else {
                    num[3] = 255;
                }
            }

        }
    }
    *size_input = pointer_otvet;
    return remainder;

} //Деление в столбик для перевода в 256сис

typedef struct {
    unsigned char arr[128];
} uint1024_t;

void scanf_value(uint1024_t *x) {
    unsigned char input[500];
    scanf("%s", &input);
    int size_input = 0;
    int fl_end = 1;
    for (int i = 0; i < 500; i++) {
        if (input[i] >= '0' && input[i] <= '9' && fl_end == 1) {
            size_input += 1;
        } else {
            fl_end = 0;
        }
    }
    int k = 127;

    while (size_input > 0) {
        x->arr[k--] = division(&size_input, input);
    }
    while (k >= 0) {
        x->arr[k--] = 0;
    }
}

void printf_value(uint1024_t x) {
    unsigned char output[500];
    unsigned char prom[500];
    int peren;
    for (int i = 0; i < 500; i++) {
        output[i] = 0;
        prom[i] = 0;
    }

    for (int i = 0; i < 128; i++) {
        int ost = x.arr[i];
        for (int y = 0; y < 500; y++) {
            prom[y] = output[y];
        }
        for (int j = 0; j < 255; j++) {
            peren = 0;
            for (int r = 499; r >= 0; r--) {
                output[r] += (prom[r] + peren);
                if (output[r] > 9) {
                    peren = 1;
                } else {
                    peren = 0;
                }
                output[r] = output[r] % 10;
            }
        }
        peren = 0;
        for (int r = 499; r >= 0; r--) {
            if (r == 499) {
                output[r] += ost % 10;
            } else if (r == 498) {
                output[r] += (ost / 10) % 10;
            } else if (r == 497) {
                output[r] += (ost / 100) % 10;
            }
            output[r] += peren;
            if (output[r] > 9) {
                peren = 1;
            } else {
                peren = 0;
            }
            output[r] = output[r] % 10;
        }

    }
    int null_pointer;
    if(output[0] == 0){
        null_pointer = 499;
    }
    else{
        null_pointer = 0;
    }
    for (int i = 0; i < 500; i++) {
        if (output[i] != 0){
            null_pointer = i;
            break;
        }
    }
    for (int i = null_pointer; i < 500; i++) {
        printf("%d", output[i]);
    }
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t res;
    unsigned char peren = 0;
    for (int i = 127; i >= 0; i--) {
        res.arr[i] = x.arr[i] + y.arr[i] + peren;
        if (x.arr[i] + y.arr[i] + peren > 255) {
            peren = 1;
        } else {
            peren = 0;
        }
    }
    return res;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t res;
    unsigned char peren = 0;
    for (int i = 127; i >= 0; i--) {
        res.arr[i] = x.arr[i] - y.arr[i] - peren;
        if (x.arr[i] - y.arr[i] - peren < 0) {
            peren = 1;
        } else {
            peren = 0;
        }
    }
    return res;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    unsigned char res2[256];
    uint1024_t res;
    for (int i = 0; i < 256; i++) {
        res2[i] = 0;
    }
    unsigned char mult[256];
    unsigned char shift = 0;
    int peren;
    for (int i = 127; i >= 0; i--) {
        peren = 0;
        unsigned char mn = y.arr[i];
        for (int j = 255; j > 255 - shift; j--) {
            mult[j] = 0;
        }
        for (int j = 127; j >= 0; j--) {
            unsigned char mn2 = x.arr[j];
            mult[128 + j - shift] = mn * mn2 + peren;
            peren = (mn * mn2 + peren) / 256;
        }
        for (int j = 127 - shift; j >= 0; j--) {
            mult[j] = 0;
        }
        unsigned char add_peren = 0;
        for (int j = 255; j >= 0; j--) {
            int fl_add_peren = res2[j] + mult[j] + add_peren;
            res2[j] += (mult[j] + add_peren);
            if (fl_add_peren > 255) {
                add_peren = 1;
            } else {
                add_peren = 0;
            }
        }
        shift++;
    }
    for (int i = 128; i < 256; i++) {
        res.arr[i - 128] = res2[i];
    }
    return res;
}

uint1024_t from_uint(unsigned int x) {
    uint1024_t a;
    int x2 = x;
    int size_input = 0;
    unsigned char input[500];
    int x_size = x;
    while (x_size > 0) {
        x_size /= 10;
        size_input++;
    }
    x_size = size_input;
    while (x > 0) {
        input[x_size - 1] = ((x % 10) + 48);
        x /= 10;
        x_size--;
    }
    if (x2 == 0) {
        size_input = 1;
        input[0] = '0';
    }
    int k = 127;

    while (size_input > 0) {
        a.arr[k--] = division(&size_input, input);
    }
    while (k >= 0) {
        a.arr[k--] = 0;
    }

    return a;
}

int main() {
    uint1024_t a;
    uint1024_t b;
    scanf_value(&a);
    scanf_value(&b);
    printf("add_op  ");
    printf_value(add_op(a, b));
    printf("\n");
    printf("subtr_op  ");
    printf_value(subtr_op(a, b));
    printf("\n");
    printf("mult_op  ");
    printf_value(mult_op(a, b));
    printf("\n");

}