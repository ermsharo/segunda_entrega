#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data
{
    char column1[100];
    char column2[100];
};

int main()
{
    FILE *file = fopen("file.csv", "r");
    if (file == NULL)
    {
        printf("Failed to open the CSV file.\n");
        return 1;
    }

    char line[256];
    struct Data data;

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        if (token != NULL)
        {
            strcpy(data.column1, token);
            token = strtok(NULL, ",");
            if (token != NULL)
            {
                strcpy(data.column2, token);

             printf("%s, %s\n", data.column1, data.column2);
            }
        }
    }
    printf("ENTREGA 2");
     return 0;
}