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
char *get_native_data(const char *string, const enum TYPE vtype, const size_t start, const size_t end)
{
    char *data = (char*)malloc(end - start);
    int end_inx = 0;

    if (vtype == N) {

        for (int i = start; i <= end; i++) {
            if (string[i] == '\0') break;

            if (string[i] == '=') {
                for (int j = i + 1; j <= end; j++) {
                    if (string[j] == '=' || isalpha(string[j]) || ispunct(string[j])) break;
                    if (isdigit(string[j])) {
                        data[end_inx] = string[j];

                        end_inx++;
                        data[end_inx] = '\0';
                    }
                }
                break;
            }
        }
    } 
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
parse check_string(const char *string, const size_t size, const enum TYPE vtype)
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
            if (vtype == S)
                result.data = get_native_data(string, S, strlen(result.var_name), size);
            else if (vtype == N)
                result.data = get_native_data(string, N, strlen(result.var_name), size);

        default: break;
    }

    return result;
}

void get_str_var(FILE *file, const char *var_name, char *data_c)
{
    char string[PBUFFER];
    parse data;

    rewind(file);
    while (fgets(string, PBUFFER, file)) {
        data = check_string(string, strlen(string), S);
        if (data.var_name == NULL && data.data == NULL) continue;

        if (!strcmp(data.var_name, var_name)) {
            memmove(data_c, data.data, strlen(data.data));

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

    rewind(file);
    while (fgets(string, PBUFFER, file)) {
        data = check_string(string, strlen(string), N);
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
