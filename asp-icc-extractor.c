#include <stdio.h>

int main(int argc, char **argv) {
    FILE *asp = fopen("AfterShot", "rb");
    if (asp!=NULL) {
        fclose (asp);
    }
    return 0;
}
