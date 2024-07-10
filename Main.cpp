#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 255
#define MAX_FILES 100
#define MAX_CONTENT_LENGTH 1024
#define MAX_CHILD_FOLDERS 100

typedef struct File
{
    char name[MAX_NAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    size_t size;
    time_t creation_date;
} File;

typedef struct Folder
{
    char name[MAX_NAME_LENGTH];
    struct Folder* Parent;
    struct Folder* children[MAX_CHILD_FOLDERS];  // Array to hold child folders
    int number_of_children;
    int number_of_files;
    File files[MAX_FILES];
};

Folder* initilize_file_system();
void handle_command(Folder* folder);

int main()
{
    Folder* root = initilize_file_system();
    handle_command(root);
    free(root);
    return 0;
}

//Root Folder
Folder* initilize_file_system() {
    Folder* rootfolder = (Folder*)malloc(sizeof(Folder));
    if (rootfolder == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }
    strcpy(rootfolder->name, "/");
    rootfolder->Parent = NULL;
    rootfolder->number_of_children = 0;
    rootfolder->number_of_files = 0;\
    for (int i; i <  MAX_FILES; i++)
    {
        rootfolder->files[i].name[0] = '\0';
        rootfolder->files[i].content[0] = '\0';
        rootfolder->files[i].creation_date =  0;
        rootfolder->files[i].size = 0;
    }
    for (int i; i <  MAX_CHILD_FOLDERS; i++)
    {
        rootfolder->children[i] = NULL;

    }
    return rootfolder;
}

void handle_command(Folder* folder)
{
    char command[256];
    char arg1[256];
    char arg2[1024];

    while (1)
    {
        printf(">");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
    }

}
