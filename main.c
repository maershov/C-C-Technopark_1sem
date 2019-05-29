#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>


#define READ_CONST_FOR_ITERATION 100;

void reverse(char *, int);

void swap(char *, char *);

int itoa(size_t, char *, int, int);

size_t toten(char *, int, size_t *);

size_t todegree(int, int);

int read(char *str, int *, int *);

int main() {

    const int num = READ_CONST_FOR_ITERATION;
    int P = 0;
    int Q = 0;
    int check = 0;

    size_t valueten = 0;

    char *str = (char *) malloc(sizeof(char) * (num + 1));
    if (str == NULL) {
        free(str);
        printf("%s", "[error]");
        return 0;
    }


    check = read(str, &P, &Q);
    if (check == -1) {
        return 0;
    }

    //printf("Q: %d P: %d str : %s", Q, P, str);

    int error_code1 = toten(str, P, &valueten);
    if (error_code1) {
        printf("%s", "[error]");
        free(str);
        return 0;
    }

    char *str1 = (char *) malloc(sizeof(char) * (num + 1));

    if (str1 == NULL) {
        free(str);
        printf("%s", "[error]");
        return 0;
    }

    int error_code = itoa(valueten, str1, Q, num);

    if (error_code) {
        printf("%s", "[error]");
        free(str1);
        free(str);
        return 0;
    }

    printf("%s", str1);

    free(str);
    free(str1);

    return 0;
}

int read(char *str, int *P, int *Q) {

    int num = READ_CONST_FOR_ITERATION;
    int reserve = READ_CONST_FOR_ITERATION
    +1;

    int position = 0;
    size_t valueten = 0;
    int r;
    *P = *Q = r = 0;
    char *buf;


    r = scanf("%d %d %100s", P, Q, str);

    if (r != 3) {
        free(str);
        printf("%s",
               "[error]");    // как проверить на другое количество введенных данных? 5 4 1 2 читает только первые 3 и все окей
        return -1;
    }

    if (!((2 <= *Q) && (*Q <= *P) && (*P <= 36))) {
        printf("%s", "[error]");
        free(str);
        return -1;
    }

    while ((strlen(str) == (num))) {
        num += READ_CONST_FOR_ITERATION;
        buf = (char *) realloc(str, sizeof(char) * (num + 1));
        if (!buf) {

            free(str);
            printf("%s", "[error]");
            return -1;
        }
        str = buf;
        char *tbuf = (char *) malloc(reserve);
        position += READ_CONST_FOR_ITERATION;
        scanf("%100s", tbuf);
        if (strlen(tbuf)) {
            memcpy((str + position), tbuf, reserve);

        }
        free(tbuf);
    }

    return 0;


}

size_t todegree(const int value, const int degree) {
    size_t newvalue = 0;
    int counter = degree;

    if (degree == 0)
        return 1;
    else
        newvalue = todegree(value, --counter) * value;

    return newvalue;
}

void swap(char *a, char *b) {
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(char str[], const int length) {
    int start = 0;
    int end = length - 1;

    while (start < end) {
        swap(&(str[start]), &(str[end]));
        start++;
        end--;
    }
}

int itoa(size_t num, char *str, const int base, const int cap) {
    int i = 0;
    int capacity = num;


    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return 0;
    }

    while (num != 0) {
        int rem = num % base;
        if (i == capacity) {
            char *tstr;
            capacity *= 2;
            tstr = (char *) realloc(str, sizeof(char) * (capacity + 1));
            if (!tstr) {

                free(str);
                printf("%s", "[error]");
                return 1;
            }
            str = tstr;
        }
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    str[i] = '\0';
    reverse(str, i);

    return 0;
}

size_t toten(char *str, const int base, size_t *valueten) {
    size_t newvalue = 0;
    size_t buf = 0;
    int position = 0;
    size_t temp = 0;
    int length = strlen(str);
    if (!length)
        return 0;
    while (length != 0) {

        buf = (str[length - 1] > '9') ? ((char) (tolower(str[length - 1])) - 'a' + 10) : (
                (char) tolower((str[length - 1])) - '0');

        if (buf > (base - 1)) {
            return 1;
        }
        temp = buf * todegree(base, position);

        if (!((SIZE_MAX - temp) > newvalue)) {
            return 2;
        }
        newvalue += temp;
        position++;
        length--;
    }

    *valueten = newvalue;
    return 0;
}
