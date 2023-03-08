#include <string.h>
#include <stdio.h>
#include "word_counter.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("USAGE: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: input file open error\n");
        return 1;
    }

    struct word_counter_t *wc = word_counter_create(50000);
    {
        const char sep[] = " ,.!:;[]()";
        char buff[256];
        while (!feof(fp)) {
            if (fscanf(fp, "%255s", buff) > 0) {
                char *word = strtok(buff, sep);
                while (word != NULL) {
                    word_counter_add(wc, word);
                    word = strtok(NULL, sep);
                }
            }
        }

        fclose(fp);
    }
    word_counter_print(wc);
    word_counter_destroy(wc);
    
    return 0;
}
