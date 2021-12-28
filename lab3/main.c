#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int arr[1900001];

void parsing(int *number_errors, int *number_lines) {
    FILE *f = fopen("F:\\C\\labs\\lab3\\access_log_Jul95.txt", "r");
    if (f == NULL) {
        printf("Error");
        exit(0);
    }

    char str[500];
    int fl;
    while (!feof(f)) {
        fgets(str, 500, f);
        arr[*number_lines] = 0;
        fl = 0;
        for (int i = 0; i < strlen(str) - 1; ++i) {
            if (str[i] == '[') {
                arr[*number_lines] += (((str[i + 1] - 48) * 10 + (str[i + 2] - 48) - 1) * 24) * 3600;
                fl = 1;
            }
            if (str[i] == ':' && fl == 1) {
                arr[*number_lines] += ((str[i + 1] - 48) * 10 + str[i + 2] - 48) * 3600 +
                                      ((str[i + 4] - 48) * 10 + str[i + 5] - 48) * 60 +
                                      ((str[i + 7] - 48) * 10 + str[i + 8] - 48);
                fl = 0;
            }
            if (str[i] == '\"' && str[i - 1] != ' ' &&
                (((str[i + 2] - 48) * 100 + (str[i + 3] - 48) * 10 + str[i + 4] - 48) > 499 &&
                 ((str[i + 2] - 48) * 100 + (str[i + 3] - 48) * 10 + str[i + 4] - 48) < 512)) {
                (*number_errors)++;
                printf("%s", str);
                break;
            }
        }
        (*number_lines)++;
    }
}

int binary_search(int i, int r, int diff) {
    int l = -1;
    int m;
    while (r - l > 1) {
        m = (l + r) / 2;
        if (arr[m] > arr[i] + diff) {
            r = m;
        } else {
            l = m;
        }
    }
    return r - i;
}


int main() {
    int dur;
    int number_lines = 0;
    int number_errors = 0;
    int number_requests = INT_MIN;
    printf("Input duration:\n");
    scanf(" %d", &dur);
    parsing(&number_errors, &number_lines);
    for (int i = 0; i < number_lines; i++) {
        if (number_requests < binary_search(i, number_lines, dur - 1)) {
            number_requests = binary_search(i, number_lines, dur - 1);
        }
    }
    printf("Number of errors:  %d\n", number_errors);
    printf("Max number of requests in %d seconds:  %d\n", dur, number_requests);

}
