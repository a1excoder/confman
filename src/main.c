#include <stdio.h>

extern char ERRORBUFFER[100];
extern void get_str_var(FILE *file, const char *var_name, char *data_c);
extern int get_int_var(FILE *file, const char *var_name);

int main(int count, const char *data[])
{
    if (count <= 1) {
        puts("Enter config file name nearby binary file");
        return 1;
    }

    FILE *file = fopen(data[1], "rt");
    if (file == NULL) {
        puts("[ERROR] - file not found.");
        return 2;
    }

    char data1[100];
    get_str_var(file, "vv1", data1);
    printf("%s\n", data1);

    printf("\n%s\n", ERRORBUFFER);

    int data2 = get_int_var(file, "iv2");
    printf("%d\n", data2);

    printf("\n%s\n", ERRORBUFFER);

    fclose(file);
    return 0;
}
