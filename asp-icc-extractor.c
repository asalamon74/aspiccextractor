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

// sometime can camera name is repeated
void cleanCameraName(char *name) {
    int len = strlen(name);
    if (strncmp(name, name+(len/2), len/2)==0) {
        name[len/2] = '\0';
    }
}

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
            searchpos = findNext(searchpos+1, 1024, "desc");
            searchpos = findNext(searchpos+1, 1024, "desc");
            searchpos = findNext(searchpos+1, 1024, "desc");
            char *cameraNamePos = findNext(searchpos+1, 1024, "desc");
            if (cameraNamePos != NULL ) {
                char *cameraName = descReader(cameraNamePos);
                cleanCameraName(cameraName);
                printf("found %s\n", cameraName);
            } else {
                printf("cannot find\n");
            }
            char *searchpos2 = searchpos+1;
            while (searchpos2 < buffer+length) {
                if (strncmp(searchpos2, "END_DATA\x0A", 9)==0) {
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
