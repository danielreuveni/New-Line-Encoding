/*
daniel reuveni
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyFile(char* argv[]);
void changeEndLine(char* argv[]);
void swapBytes(char* argv[]);

int main(int argc, char* argv[]) {
    //option 1
    if (argc == 3) {
        //if the name of files are equal. dont copy
        if (!strcmp(argv[1], argv[2]))
            return 0;
        copyFile(argv);
    }

    //option 2
    if (argc == 5) {
        //if the name of files are equal, and the flags are equal, dont copy
        if (!strcmp(argv[1], argv[2]) && !strcmp(argv[3], argv[4]))
            return 0;
        changeEndLine(argv);
    }

    //option 3
    if (argc == 6) {
        //keep the file, dont copy
        if (!strcmp(argv[1], argv[2]) && !strcmp(argv[3], argv[4]) && !strcmp(argv[5], "-keep"))
            return 0;
        if (!strcmp(argv[5], "-keep"))
            changeEndLine(argv);
        if (!strcmp(argv[5], "-swap")) {
            changeEndLine(argv);
            //FILE *file_ptr = fopen(argv[2], "wb");
            swapBytes(argv);
        }
    }
    else
        return 0;
}
// this funcion copy the chars of the file, char by char
void copyFile(char* argv[]) {
    //two pointers of the 2 files
    FILE* file_ptr1, * file_ptr2;
    char a;
    file_ptr1 = fopen(argv[1], "rb");
    if (file_ptr1 == NULL)
        return;
    file_ptr2 = fopen(argv[2], "wb");
    if (file_ptr2 == NULL)
        return;
    if (!strcmp(argv[1], argv[2]))
        return;
    // get the chars from the source.txt until the end of the file
    // we should copy the bom
    a = fgetc(file_ptr1);
    fputc(a, file_ptr2);
    a = fgetc(file_ptr1);
    fputc(a, file_ptr2);
    a = fgetc(file_ptr1);
    while (a != EOF) {
        fputc(a, file_ptr2);
        //printf("!");
        a = fgetc(file_ptr1);
    }

    fclose(file_ptr1);
    fclose(file_ptr2);
}

// this function convert the txt file from windows to unix or mac
void fromWinTo(char* string, FILE* file_ptr1, FILE* file_ptr2);
// this function convert the txt file from unix to windows or mac
void fromUnixTo(char* string, FILE* file_ptr1, FILE* file_ptr2);
// this function convert the txt file from mac to unix or windows
void fromMacTo(char* string, FILE* file_ptr1, FILE* file_ptr2);

//this function change the endline accorfing to mmaarchet hafaala and call to other functions to change
void changeEndLine(char* argv[]) {

    FILE* file_ptr1, * file_ptr2;
    char a;
    file_ptr1 = fopen(argv[1], "rb");
    if (file_ptr1 == NULL)
        return;
    file_ptr2 = fopen(argv[2], "wb");
    if (file_ptr2 == NULL)
        return;

    if (!strcmp(argv[3], "-win") && !strcmp(argv[4], "-unix"))
    {
        fromWinTo("unix", file_ptr1, file_ptr2);
    }
    if (!strcmp(argv[3], "-win") && !strcmp(argv[4], "-mac")) {
        fromWinTo("mac", file_ptr1, file_ptr2);
    }
    if (!strcmp(argv[3], "-unix") && !strcmp(argv[4], "-win")) {
        fromUnixTo("win", file_ptr1, file_ptr2);
    }
    if (!strcmp(argv[3], "-unix") && !strcmp(argv[4], "-mac")) {
        fromUnixTo("mac", file_ptr1, file_ptr2);
    }
    if (!strcmp(argv[3], "-mac") && !strcmp(argv[4], "-unix")) {
        fromMacTo("unix", file_ptr1, file_ptr2);
    }
    if (!strcmp(argv[3], "-mac") && !strcmp(argv[4], "-win")) {
        fromMacTo("win", file_ptr1, file_ptr2);
    }
    
    if (!strcmp(argv[3], argv[4])) {
        copyFile(argv);
    }
    fclose(file_ptr1);
    fclose(file_ptr2);
    return;
}

void fromWinTo(char* string, FILE* file_ptr1, FILE* file_ptr2) {
    char a;
    char bom;
    //BOM
    a = fgetc(file_ptr1);
    bom = a;
    fputc(a, file_ptr2);
    a = fgetc(file_ptr1);
    if (a == EOF) {
        fputc(a, file_ptr2);
        a = fgetc(file_ptr1);
    }
    //diffrence beteen big and little endian
    if (bom == 0xfffffffe) {
        while (a != EOF) {
            //recognize the end line according to windows
            if (a == 0x00) {
                a = fgetc(file_ptr1);
                if (a == 0x000d) {
                    a = fgetc(file_ptr1);
                    if (a == 0x00) {
                        a = fgetc(file_ptr1);
                        if (a == 0x000a) {
                            a = 0x00;
                            fputc(a, file_ptr2);
                            //change
                            if (!strcmp(string, "unix"))
                                a = 0x000a;
                            if (!strcmp(string, "mac"))
                                a = 0x000d;
                        }
                    }
                }
                else {
                    fputc(0x00, file_ptr2);
                }
            }

            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }

    if (bom == EOF) {
        while (a != EOF) {
            //recognize the end line according to windows
            if (a == 0x000d) {
                a = fgetc(file_ptr1);
                if (a == 0x00) {
                    a = fgetc(file_ptr1);
                    if (a == 0x000a) {
                        a = fgetc(file_ptr1);
                        if (a == 0x00) {
                            if (!strcmp(string, "unix"))
                                a = 0x000a;
                            if (!strcmp(string, "mac"))
                                a = 0x000d;
                            fputc(a, file_ptr2);
                        }
                        a = 0x00;
                    }
                }
            }
            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }
    //} 
}

void fromUnixTo(char* string, FILE* file_ptr1, FILE* file_ptr2) {
    //if (!strcmp(string, "-unix")) {
    char a;
    char bom;
    //BOM
    a = fgetc(file_ptr1);
    bom = a;
    fputc(a, file_ptr2);
    a = fgetc(file_ptr1);
    if (a == EOF) {
        fputc(a, file_ptr2);
        a = fgetc(file_ptr1);
    }
    //diffrence beteen big and little endian
    if (bom == 0xfffffffe) {
        while (a != EOF) {
            if (a == 0x00) {
                a = fgetc(file_ptr1);
                //recognize the end line according to unix
                if (a == 0x000a) {
                    a = 0x00;
                    fputc(a, file_ptr2);
                    //change
                    if (!strcmp(string, "win")) {
                        a = 0x000d;
                        fputc(a, file_ptr2);
                        a = 0x00;
                        fputc(a, file_ptr2);
                        a = 0x000a;
                    }
                    if (!strcmp(string, "mac"))
                        a = 0x000d;
                }
                else {
                    fputc(0x00, file_ptr2);
                }
            }



            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }

    if (bom == EOF) {
        while (a != EOF) {
            //recognize the end line according to unix
            if (a == 0x000a) {
                a = fgetc(file_ptr1);
                if (a == 0x00) {

                    if (!strcmp(string, "win")) {
                        a = 0x000d;
                        fputc(a, file_ptr2);
                        a = 0x00;
                        fputc(a, file_ptr2);
                        a = 0x000a;
                    }
                    if (!strcmp(string, "mac"))
                        a = 0x000d;
                    fputc(a, file_ptr2);
                }
                a = 0x00;
            }
            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }
    //} 
}

void fromMacTo(char* string, FILE* file_ptr1, FILE* file_ptr2) {
    char a;
    char bom;
    //BOM
    a = fgetc(file_ptr1);
    bom = a;
    fputc(a, file_ptr2);
    a = fgetc(file_ptr1);
    if (a == EOF) {
        fputc(a, file_ptr2);
        a = fgetc(file_ptr1);
    }

    //diffrence beteen big and little endian
    //little endian
    if (bom == 0xfffffffe) {
        while (a != EOF) {
            //recognize the end line according to mac
            if (a == 0x00) {
                a = fgetc(file_ptr1);
                if (a == 0x000d) {
                    a = 0x00;
                    fputc(a, file_ptr2);
                    if (!strcmp(string, "win")) {
                        a = 0x000d;
                        fputc(a, file_ptr2);
                        a = 0x00;
                        fputc(a, file_ptr2);
                        a = 0x000a;
                    }
                    if (!strcmp(string, "unix"))
                        a = 0x000a;
                }
                else {
                    fputc(0x00, file_ptr2);
                }
            }



            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }
    //big endian
    if (bom == EOF) {

        while (a != EOF) {
            if (a == 0x000d) {
                a = fgetc(file_ptr1);
                if (a == 0x00) {
                    if (!strcmp(string, "win")) {
                        a = 0x000d;
                        fputc(a, file_ptr2);
                        a = 0x00;
                        fputc(a, file_ptr2);
                        a = 0x000a;
                        //fputc(a, file_ptr2);
                    }
                    if (!strcmp(string, "unix"))
                        a = 0x000a;
                    fputc(a, file_ptr2);
                }
                a = 0x00;
            }
            if (a != EOF)
                fputc(a, file_ptr2);
            a = fgetc(file_ptr1);
        }
    }
    //} 
}
// this function swap the two bytes of the char between them
void swapBytes(char* argv[]) {
    FILE* file_ptr1, * file_ptr2;
    char a;
    file_ptr1 = fopen(argv[2], "rb");
    char arr[2];
    if (file_ptr1 == NULL)
        return;
    //copy the nrw file after changing the endline
    file_ptr2 = fopen("tmp.txt", "wb");
    if (file_ptr2 == NULL)
        return;

    a = fgetc(file_ptr1);
    arr[0] = a;
    a = fgetc(file_ptr1);
    arr[1] = a;
    //swap the  first bytes
    fputc(arr[1], file_ptr2);
    fputc(arr[0], file_ptr2);
    a = fgetc(file_ptr1);
    while (a != EOF) {
        arr[0] = a;
        a = fgetc(file_ptr1);
        arr[1] = a;
        //swap again
        fputc(arr[1], file_ptr2);
        fputc(arr[0], file_ptr2);
        a = fgetc(file_ptr1);
    }

    fclose(file_ptr1);
    fclose(file_ptr2);
    //copy file function work according to the arguments of the cmd. the important arguments is the
    // second and third
    char* arg[3] = { "0", "tmp.txt", argv[2] };
    //copy the temp file to the destination file
    copyFile(arg);
    //rename("tmp.txt", argv[2]);
}
