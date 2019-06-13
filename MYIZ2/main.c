
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_CONST 1001;

#define READ_CONST 11;


struct buffer {
    char *str;
    int flag;
};

int reversed_polish_notation(char *vr, char *exit); //func makes reversed p.n. from the expression

int calculating(char *str_calc);//func calculate rpn

int delete_space(char *str);//just delete spaces out of commas

int line_balance_check(char *line);//balanced parenthesis

int command_handler(char *str_calc, struct buffer *my_stack);

int plus_command(struct buffer *my_stack, char *buf, char *str_buf, int buffer_const, int *p);

int multiply_command(struct buffer *my_stack, char *buf, char *str_buf, int buffer_const, int *p, int *capacity);

int quote_command(char *str_calc, int *j, char *buf, struct buffer *my_stack, int buffer_const, int *p);

char *read_function(char *str, int num, int read_const, int position, int read);


int main(void) {


    const int buffer_const = BUFFER_CONST;
    const int read_const = READ_CONST;


    char *exit = (char *) malloc(sizeof(char) * buffer_const);
    if (exit == NULL) {
        free(exit);
        return 0;
    }

    memset(exit, 0, buffer_const);

    int num = read_const;
    int position = 0;
    int read = read_const;

    char *str = (char *) malloc(sizeof(char) * (num + 1));

    if (str == NULL) {
        free(str);
        free(exit);
        return 0;
    }

    char *new_str = (char *) malloc(sizeof(char) * buffer_const);


    if (new_str == NULL) {
        free(str);
        free(new_str);
        free(exit);
        return 0;
    }

    int number = 0;
    int error_calc = 0;

    str = read_function(str, num, read_const, position, read);


    if(str == NULL){
        free(str);
        free(new_str);
        free(exit);
        return 0;
    }

    number = delete_space(str);

    if (number == -1) {
        free(str);
        free(exit);
        free(new_str);
        printf("%s", "[error]");
        return 0;
    }

    if ((number % 2)) {//check balanced commas
        free(str);
        free(exit);
        free(new_str);
        printf("%s", "[error]");
        return 0;
    }


    strcpy(new_str, str);
    int error_check_line = line_balance_check(new_str);
    if (!error_check_line) {
        free(str);
        free(exit);
        free(new_str);
        printf("%s", "[error]");
        return 0;
    }

    if (error_check_line == -1) {   //zero pointer
        free(str);
        free(exit);
        free(new_str);
        printf("%s", "[error]");
        return 0;
    }


    reversed_polish_notation(str, exit);

    error_calc = calculating(exit);

    if (error_calc == 1) { //malloc error
        free(str);
        free(exit);
        free(new_str);
        return 0;
    }

    if (error_calc == 2) {  //zero pointer
        free(str);
        free(exit);
        free(new_str);
        return 0;
    }

    free(str);
    free(exit);
    free(new_str);

    return 0;
}

char *read_function(char *str, int num, int read_const, int position, int read) {

    if (str == NULL){
        printf("%s", "[error]");
        return NULL;

    }

    fgets(str, num, stdin);

    while ((num - 1) == strlen(str)) {


        char *tbuf = (char *) malloc(sizeof(char) * (num + 1));

        if (tbuf == NULL) {
            free(tbuf);
            free(str);
            printf("%s", "[error]");
            return NULL;
        }

        num += (read_const - 1);

        char *tstr;
        tstr = realloc(str, sizeof(char) * (num));

        if (!tstr) {
            free(str);
            free(tbuf);
            printf("%s", "[error]");
            return NULL;
        }

        str = tstr;
        position += (read_const - 1);
        fgets(tbuf, read, stdin);

        memcpy((str + position), tbuf, read);

        free(tbuf);
    }

    return str;

}


int reversed_polish_notation(char *vr, char *exit) {

    int i = 0, s = 0, e = 0;
    int buffer_const = BUFFER_CONST;
    char stek[buffer_const];

    if ((vr == NULL) || (exit == NULL)) {
        return 1;
    }
    for (i = 0; vr[i] != '\0'; i++)
        switch (vr[i]) {
            case '+':

                while ((s != 0) && (stek[s - 1] != '(')) {
                    exit[e] = stek[s - 1];
                    e++;
                    s--;
                }

                stek[s] = '+';
                s++;
                break;

            case '*':

                while ((s != 0) && (stek[s - 1] != '(') && (stek[s - 1] != '+')) {
                    exit[e] = stek[s - 1];
                    e++;
                    s--;
                }

                stek[s] = '*';
                s++;
                break;

            case '(': {
                stek[s] = '(';
                s++;
                break;
            }
            case ')':
                while ((stek[s - 1] != '(') && (s != 0)) {
                    exit[e] = stek[s - 1];
                    e++;
                    s--;
                }
                stek[s - 1] = ' ';
                s--;
                break;

            default: {
                exit[e] = vr[i];
                e++;
                if ((vr[i + 1] == '+') ||
                    (vr[i + 1] == '*') ||
                    (vr[i + 1] == '(') ||
                    (vr[i + 1] == ')')) {
                    exit[e] = ' ';
                    e++;
                }
            }
        }
    exit[e] = ' ';
    e++;

    while (s != 0) {
        exit[e] = stek[s - 1];
        e++;
        s--;
    }

    return i;
}


int calculating(char *str_calc) {


    int buffer_const = BUFFER_CONST;


    if (str_calc == NULL) {
        return 2;
    }
    struct buffer *my_stack = (struct buffer *) malloc(sizeof(struct buffer) * buffer_const);
    if (my_stack == NULL) {
        free(my_stack);
        return 1;
    }

    for (int i = 0; i < buffer_const; i++) {
        my_stack[i].str = (char *) malloc(sizeof(char) * buffer_const);
        if (my_stack[i].str == NULL) {
            break;
            for (int j = 0; j < i; ++j) {
                free(my_stack[j].str);
            }
            free(my_stack);
            return 1;
        }
        memset(my_stack[i].str, 0, buffer_const);
        my_stack[i].flag = 0;
    }

    int command_error = command_handler(str_calc, my_stack);

    if (command_error == 1){
        for (int i = 0; i < buffer_const; i++) {
            free(my_stack[i].str);
        }

        free(my_stack);

        return 3;

    }

    for (int i = 0; i < buffer_const; i++) {
        free(my_stack[i].str);
    }

    free(my_stack);

    return 0;
}

int command_handler(char *str_calc, struct buffer *my_stack) {


    int p = 0;
    int buffer_const = BUFFER_CONST;
    int capacity = buffer_const;

    if (str_calc == NULL) {
        return 2;
    }

    char *buf = (char *) malloc(sizeof(char) * buffer_const);
    if (buf == NULL) {

        free(buf);
        return 1;
    }
    char *str_buf = (char *) malloc(sizeof(char) * buffer_const);
    if (str_buf == NULL) {

        free(buf);
        free(str_buf);
        return 1;
    }
    memset(str_buf, 0, buffer_const);
    memset(buf, 0, buffer_const);

    for (int j = 0; str_calc[j] != '\0'; j++) {

        if (str_calc[j] == '\0')
            break;

        else if (str_calc[j] == ' ' || str_calc[j] == '\n')
            continue;

        else if (str_calc[j] == '+') {

            int error_plus = plus_command(my_stack, buf, str_buf, buffer_const, &p);
            if (error_plus) {
                if (error_plus == 1){
                    free(buf);
                    free(str_buf);
                }

                return 1;

            }
            continue;

        } else if (str_calc[j] == '*') {
            int error_multiply = multiply_command(my_stack, buf, str_buf, buffer_const, &p, &capacity);
            if (error_multiply) {
                if (error_multiply == 1){

                    free(buf);
                    free(str_buf);

                }

                return 1;
            }
            continue;

        } else {

            if (str_calc[j] == '"') {

                int error_quote = quote_command(str_calc, &j, buf, my_stack, buffer_const, &p);
                if (error_quote) {

                    free(buf);
                    free(str_buf);
                    return 1;
                }
                continue;

            } else {

                if (isdigit(str_calc[j])) {

                    int i = 0;

                    while (isdigit(str_calc[j])) {
                        buf[i] = str_calc[j];
                        j++;
                        i++;
                    }
                }

                strcpy(my_stack[p].str, buf);
                my_stack[p].flag = 1;
                p++;
                memset(buf, 0, buffer_const);
                continue;
            }

        }


    }
    free(buf);
    free(str_buf);
    printf("\"%s\"", my_stack[p - 1].str);

}

int plus_command(struct buffer *my_stack, char *buf, char *str_buf, int buffer_const, int *p) {

    if ((str_buf == NULL) || (buf == NULL) || (my_stack == NULL) || (p == NULL)) {
        printf("%s", "[error]");
        return 1;
    }

    if ((my_stack[*p - 2].flag) == 1 || (my_stack[*p - 1].flag) == 1) {

        printf("%s", "[error]");
        return 1;
    }
    strcpy(str_buf, my_stack[*p - 2].str);
    strcat(str_buf, my_stack[*p - 1].str);
    memset(my_stack[*p - 2].str, 0, buffer_const);
    strcpy(my_stack[*p - 2].str, str_buf);
    memset(str_buf, 0, buffer_const);
    my_stack[*p - 2].flag = 0;
    (*p)--;


    return 0;

}

int multiply_command(struct buffer *my_stack, char *buf, char *str_buf, int buffer_const, int *p, int *capacity) {

    if ((capacity == NULL) || (str_buf == NULL) || (buf == NULL) || (my_stack == NULL) || (p == NULL)) {
        printf("%s", "[error1]");
        return 1;
    }


    int counter = 0;
    int res = 0;

    if (my_stack[*p - 2].flag) {
        res = 1;


        counter = atoi(my_stack[*p - 2].str);

        if (my_stack[*p - 2].str[0] != '0') {

            if (counter == 0) {

                printf("%s", "[error2]");
                return 1;
            }
        }

        if (my_stack[*p - 2].str[0] == '"') {

            printf("%s", "[error3]");
            return 1;

        }

        char new_buffer[buffer_const];

        strcpy(new_buffer, my_stack[*p - 1].str);
        memset(my_stack[*p - 2].str, 0, buffer_const);

        while (strlen(new_buffer) * counter > (*capacity)) {
            char *tstr;
            (*capacity) *= 3;
            tstr = realloc(my_stack[*p - 2].str, sizeof(char) * (*capacity));

            if (!tstr) {
                free(my_stack[*p - 2].str);
                printf("%s", "[error4]");
                return 2;
            }
            my_stack[*p - 2].str = tstr;
        }

        for (int i = 0; i < counter; i++) {
            strcat(my_stack[*p - 2].str, new_buffer);
        }
        my_stack[*p - 2].flag = 0;
    }

    if (my_stack[*p - 1].flag) {
        res = 1;
        counter = atoi(my_stack[*p - 1].str);

        if (my_stack[*p - 1].str[0] != '0') {

            if (counter == 0) {

                printf("%s", "[error5]");
                return 1;
            }
        }

        if (my_stack[*p - 1].str[0] == '"') {

            printf("%s", "[error6]");
            return 1;

        }

        char new_buffer[buffer_const];
        strcpy(new_buffer, my_stack[*p - 2].str);
        memset(my_stack[*p - 2].str, 0, buffer_const);

        while (strlen(new_buffer) * counter > (*capacity)) {
            char *tstr;
            (*capacity) *= 3;
            tstr = realloc(my_stack[*p - 2].str, sizeof(char) * (*capacity));

            if (!tstr) {
                free(my_stack[*p - 2].str);
                printf("%s", "[error]");
                return 2;
            }

            my_stack[*p - 2].str = tstr;
        }

        for (int i = 0; i < counter; i++) {
            strcat(my_stack[*p - 2].str, new_buffer);
        }

        my_stack[*p - 2].flag = 0;
    }

    if (res == 0) {

        printf("%s", "[error7]");
        return 1;
    }
    (*p)--;
    return 0;
}

int quote_command(char *str_calc, int *j, char *buf, struct buffer *my_stack, int buffer_const, int *p) {

    if ((str_calc == NULL) || (j == NULL) || (buf == NULL) || (my_stack == NULL) || (p == NULL)) {
        printf("%s", "[error]");
        return 1;
    }
    (*j)++;

    int i = 0;

    while (str_calc[*j] != '"') {

        buf[i] = str_calc[*j];
        (*j)++;
        i++;
    }

    strcpy(my_stack[*p].str, buf);
    my_stack[*p].flag = 0;
    (*p)++;
    memset(buf, 0, buffer_const);

    return 0;
}

int delete_space(char *str) {

    if (str == NULL) {
        return -1;
    }

    size_t length = strlen(str);
    int number = 0;

    char *buf = (char *) malloc(sizeof(char) * length);

    if (buf == NULL) {
        return -1;
    }

    int i = 0;
    int j = 0;
    int flag = 0;

    while (str[i] != '\0') {
        if (str[i] == '"') {
            flag++;
            number++;
        }

        if ((str[i] != ' ') || (flag == 1)) {
            buf[j] = str[i];
            j++;
            i++;
        } else
            i++;

        if (flag == 2) {
            flag = 0;
        }
    }
    buf[j] = '\0';
    memset(str, 0, length);
    memcpy(str, buf, strlen(buf) + 1);
    free(buf);

    return number;
}

int line_balance_check(char *line)   //this function check if the line is balanced parenthesis only
{

    if (line == NULL) {
        return -1;
    }
    size_t l = strlen(line);
    int parenthesis_counter = 0;    //счетчик количества скобок в строке
    int open_parenthesis_pos = 0;   //позиция открывающей скобки
    int close_parenthesis_pos = 0;  //позиция закрывающей скобки
    int open_parenthesis_flag = 0;  //данным флагом проверяется наличие открывающей скобки в строке
    int close_parenthesis_flag = 0; //данным флагом проверяется наличие закрывающей скобки в строке

    for (int i = 0; i < l; i++) {
        if ((line[i] == '(') || (line[i] == ')'))
            parenthesis_counter++;
    }
    if (parenthesis_counter == 0) {
        return 2;
    }
    if (!(parenthesis_counter % 2)) {
        for (int j = 0; j < parenthesis_counter / 2; j++) {
            for (int k = 0; k < l; k++) {
                if (line[k] == '(') {
                    open_parenthesis_pos = k;
                    open_parenthesis_flag = 1;
                }
                if (line[k] == ')') {
                    close_parenthesis_pos = k;
                    close_parenthesis_flag = 1;
                }
                if ((open_parenthesis_flag) && (close_parenthesis_flag) &&
                    (open_parenthesis_pos < close_parenthesis_pos)) {
                    line[open_parenthesis_pos] = '1';
                    line[close_parenthesis_pos] = '1';
                    open_parenthesis_flag = 0;
                    close_parenthesis_flag = 0;
                    break;
                }
            }

        }
        for (int k = 0; k < l; k++)
            if ((line[k] == '(') || (line[k] == ')'))
                return 0;
        return 1;
    } else
        return 0;


}
