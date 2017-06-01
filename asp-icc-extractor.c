#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *descReader(char *ptr) {
    long length = ptr[8] << 24 | ptr[9] << 16 | ptr[10] << 8 | ptr[11];
    char *desc = (char *)malloc((length)*sizeof(char));
    strncpy( desc, ptr+12, length);
    return desc;
}

char *findNext(char *haystack, int haystackLength, char*needle) {
    char *searchpos = haystack;
    int needleLength = strlen(needle);
    while (searchpos<haystack+haystackLength-needleLength) {
        if (strncmp(searchpos, needle, needleLength)==0) {
            return searchpos;
        }
        ++searchpos;
    }
    return NULL;
}

// sometimes camera name is repeated
void cleanCameraName(char *name) {
    int len = strlen(name);
    if (strncmp(name, name+(len/2), len/2)==0) {
        name[len/2] = '\0';
    }
}

void writeFile(char *fileName, char *buffer, int length) {
    FILE *fout = fopen(fileName, "wb");
    fwrite(buffer, length, 1,fout);
    fclose(fout);
}

void usage(char *name) {
    printf("\nUsage: %s filename\n\n\
Extract ICC camera color profiles from Corel AfterShot Pro binary.\n\
\n\
  filename: binary file of Corel AfterShot Pro\n\
\n", name);
}

int main(int argc, char **argv) {
    if (argc != 2 ) {
        usage(argv[0]);
        return 0;
    }
    FILE *asp = fopen(argv[1], "rb");
    if (asp==NULL) {
        fprintf(stderr, "Cannot open file %s\n",argv[1]);
        exit(-1);
    }
    fseek(asp, 0, SEEK_END);
    int length = ftell(asp);
    rewind(asp);
    char *buffer = (char *)malloc((length+1)*sizeof(char));
    fread(buffer, length, 1, asp);
    char *searchpos = buffer;
    char *beginpos;
    while (searchpos < buffer+length) {
        if (strncmp(searchpos, "scnrRGB XYZ", 11)==0) {
            beginpos = searchpos-12;
            searchpos = findNext(searchpos+1, 1024, "desc");
            searchpos = findNext(searchpos+1, 1024, "desc");
            char *iccName = descReader(searchpos);
            searchpos = findNext(searchpos+1, 1024, "desc");
            char *cameraNamePos = findNext(searchpos+1, 1024, "desc");
            char *cameraName = descReader(cameraNamePos);
            cleanCameraName(cameraName);
            char *searchpos2 = searchpos+1;
            while (searchpos2 < buffer+length) {
                if (strncmp(searchpos2, "END_DATA\x0A", 9)==0 || strncmp(searchpos2, "END_DATA\x0D", 9)==0) {
                    break;
                }
                ++searchpos2;
            }
            char *fname = malloc(strlen(iccName)+5);
            sprintf(fname, "%s.icc", iccName);
            printf("saving %s for camera %s\n", fname, cameraName);
            writeFile(fname, beginpos, (searchpos2-beginpos)+9);
        }
        ++searchpos;
    }
    if (asp!=NULL) {
        fclose (asp);
    }
    return 0;
}
