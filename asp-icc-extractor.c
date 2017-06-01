#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *asp = fopen("AfterShot", "rb");
    fseek(asp, 0, SEEK_END);
    int length = ftell(asp);
    rewind(asp);
    char *buffer = (char *)malloc((length+1)*sizeof(char));
    fread(buffer, length, 1, asp);
    if (asp!=NULL) {
        fclose (asp);
    }
    return 0;
}
