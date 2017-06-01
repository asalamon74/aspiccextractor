#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *asp = fopen("AfterShot", "rb");
    fseek(asp, 0, SEEK_END);
    int length = ftell(asp);
    rewind(asp);
    char *buffer = (char *)malloc((length+1)*sizeof(char));
    fread(buffer, length, 1, asp);
    char *searchpos = buffer;
    while (searchpos < buffer+length) {
        if (strncmp(searchpos, "scnrRGB XYZ", 11)==0) {
            printf("found\n");
            char *searchpos2 = searchpos+1;
            while (searchpos2 < buffer+length) {
                if (strncmp(searchpos2, "END_DATA\x0A", 9)==0) {
                    printf("found2\n");
                    break;
                }
                ++searchpos2;
            }
        }
        ++searchpos;
    }
    if (asp!=NULL) {
        fclose (asp);
    }
    return 0;
}
