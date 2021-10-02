#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PBUFFER 1024

typedef struct {
    char *var_name;
    char *data;
} parse;

enum TYPE {
    STR, NUM
} S = STR, N = NUM;


static char ERRORSYM[] = {
    '!', '@', '#', '$', '%', '^', ':', ';', '"', '?', '&', '*',
    '(', ')', '-', '+', '=', '/', ',', '`', '~'
};

// buffer for errors
char ERRORBUFFER[100];


static inline
int8_t check_name(const char *var_name, const size_t namesz)
{
    int8_t data = 0;
    for (size_t i = 0; i < namesz; i++)
        if (strchr(var_name, ERRORSYM[i]) != NULL) data++;

    return data;
}

static inline
int lenstr(const char *string)
{
    int size = 0;
    char c;
    while ((c = *(string++)) != '\0') size++;

    return size;
}

static inline
char *get_name(const char *string, const size_t start, const size_t end)
{
    char *vname = (char*)malloc(end - start);
    int end_inx = 0;

    for (int i = start+1; i <= end; i++) {
        if (string[i] == '=' || string[i] == ' ' || string[i] == '\0') break;

        vname[end_inx] = string[i];
        end_inx++;
        vname[end_inx] = '\0';
    }

    return vname;
}

static
char *get_native_data(const char *string, const enum TYPE vtype, const size_t start, const size_t end, const int str_n)
{
    char *data = (char*)malloc((end - start) + 1);
    int end_inx = 0;

    // read int type variables
    if (vtype == N) {

        for (int i = start + 1; i <= end; i++) {
            if (string[i] == '\0') break;

            if (string[i] == ' ' || string[i] == '=') {

                if (string[i] == '=') {
                    for (int j = i + 1; j <= end; j++) {
                        if (string[j] == '=' || isalpha(string[j])) break;

                        if (string[j] == '-' || isdigit(string[j])) {
                            data[end_inx] = string[j];

                            end_inx++;
                            data[end_inx] = '\0';
                        }
                    }
                    break;
                }
            } else sprintf(ERRORBUFFER, "[Syntax error] - on %d string on %d symbol", str_n, i+1);
        }
    }
    // read string type variables
    else if (vtype == S) {

        for (int i = start; i <= end; i++) {
            if (string[i] == '\0') break;

            if (string[i] == '=') {
                for (int j = i + 1; j <= end; j++) {
                    if (string[j] == '=' || isdigit(string[j]) || isalpha(string[j])) break;
                    if (string[j] == '"') {

                        for (int k = j + 1; k <= end; k++) {
                            if (string[k] == '"') break;

                            data[end_inx] = string[k];

                            end_inx++;
                            data[end_inx] = '\0';
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    return data;
}

static
parse check_string(const char *string, const size_t size, const enum TYPE vtype, const int str_n)
{
    parse result = {NULL, NULL};
    int inx = 0;

    for (size_t i = 0; i < size; i++) {
        if (string[i] != ' ') {
            inx = i;
            break;
        }
    }

    switch (string[inx]) {
        case '#': break;
        case '$':
            if (string[inx+1] == ' ') break;
            result.var_name = get_name(string, inx, size);

            // check syntax error of names variables
            if (check_name(result.var_name, sizeof(result.var_name) / sizeof(*result.var_name)))
                sprintf(ERRORBUFFER, "[Syntax error of variable name] - on %d string", str_n);

            if (vtype == S)
                result.data = get_native_data(string, S, lenstr(result.var_name), lenstr(string), str_n);
            else if (vtype == N)
                result.data = get_native_data(string, N, lenstr(result.var_name), lenstr(string), str_n);

        default: break;
    }

    return result;
}

void get_str_var(FILE *file, const char *var_name, char *data_c)
{
    char string[PBUFFER];
    parse data;
    int str_n = 0;

    rewind(file);
    while (fgets(string, PBUFFER, file)) {
        str_n++;
        data = check_string(string, PBUFFER, S, str_n);
        if (data.var_name == NULL && data.data == NULL) continue;

        if (!strcmp(data.var_name, var_name)) {
            memmove(data_c, data.data, lenstr(data.data));

            free(data.var_name);
            free(data.data);
            break;
        }
    }

}

int get_int_var(FILE *file, const char *var_name)
{
    char string[PBUFFER];
    parse data;
    int result_data;
    int str_n = 0;

    rewind(file);
    while (fgets(string, PBUFFER, file)) {
        str_n++;
        data = check_string(string, lenstr(string), N, str_n);
        if (data.var_name == NULL && data.data == NULL) continue;

        if (!strcmp(data.var_name, var_name)) {
            result_data = atoi(data.data);

            free(data.var_name);
            free(data.data);
            break;
        }
    }

    return result_data;
}
