#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int err = -1;

    if (argc < 3) {
        printf("Invalid arguments\n");
        return -1;
    }

    char *filename[2] = {
        argv[1], argv[2]
    };
    FILE *file[2];
    uint32_t filesize[2] = {0};
    uint8_t *filedata[2];

    printf("Comparing binary files...\n");

    for (size_t i = 0 ; i < 2 ; i++) {
        file[i] = fopen(filename[i], "rb");

        if (!file[i]) {
            printf("\t- Error: %s not found\n", filename[i]);
            return -1;
        }

        fseek(file[i], 0, SEEK_END);
        filesize[i] = ftell(file[i]);

        rewind(file[i]);

        printf("\t- %s - file size: %d\n", filename[i], filesize[i]);

        filedata[i] = malloc(filesize[i]);
        (void)fread(filedata[i], filesize[i], 1, file[i]);
    }

    size_t cmpsize = filesize[0] > filesize[1] ? filesize[0] : filesize[1];
    if (argc > 3) {
        cmpsize = strtoul(argv[3], NULL, 0);
    } else if (filesize[0] != filesize[1]) {
        printf("RESULT: Length mismatch!\n");
        err = -2;
        goto ending;
    }

    bool continue_on_mismatch = false;
    if (argc > 4) {
        if (strcmp(argv[4], "true") == 0) {
            continue_on_mismatch = true;
        }
    }

    err = 0;
    for (size_t i = 0 ; i < cmpsize ; i++) {
        if (filedata[0][i] != filedata[1][i]) {
            printf("RESULT: Data mismatch on byte: %ld - 0x%02X, 0x%02X\n",
                i, filedata[0][i], filedata[1][i]);
            err = -3;
            if (!continue_on_mismatch) {
                break;
            }
        }
    }

    if (!err) {
        printf("RESULT: Both files contain the same data!\n");
    }


ending:
    fclose(file[0]);
    fclose(file[1]);
    free(filedata[0]);
    free(filedata[1]);

    return 0;
}