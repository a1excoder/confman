#include <stdio.h>
#include "../lib/confman.h"

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

    int data2 = get_int_var(file, "iv2");
    printf("%d\n", data2);

    fclose(file);
    return 0;
}
