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

void handle_command(Folder* root);

void create_file(char arg1[256], char arg2[1024], Folder* folder);

void create_directory(char arg1[256], Folder* folder, char path[2000]);

void read_file(char arg1[256], Folder* folder);

void write_file(char arg1[256], char arg2[1024], Folder* folder);

void delete_file(char arg1[256], Folder* folder);

void delete_directory(char arg1[256], Folder* folder);

void list_directory_contents();

void change_directory(char arg1[256], Folder* folder);

void save_file_system(char arg1[256], Folder* folder);

void load_file_system(char arg1[256], Folder* folder);

Folder* find_folder(const char *path, Folder* root);

Folder* find_file(const char *path, Folder* root);

void all_directories(Folder *pFolder);

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
    strcpy(rootfolder->name, "root");
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

void handle_command(Folder* root)
{
    char command[256];
    char arg1[256];
    char arg2[1024];
    char path[2000];

    while (1)
    {
        printf(">");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (sscanf(command, "create file %s %s", arg1, arg2) == 2) {
            // create_file function should be defined elsewhere
            create_file(arg1, arg2, root);
        } else if (sscanf(command, "create dir %s %s", arg1, arg2) == 2) {
            // create_dir function should be defined elsewhere
            create_directory(arg1, root, arg2);
        } else if (sscanf(command, "read %s", arg1) == 1) {
            read_file(arg1, root);
        } else if (sscanf(command, "write %s %s", arg1, arg2) == 2) {
            write_file(arg1, arg2, root);
        } else if (sscanf(command, "delete file %s", arg1) == 1) {
            delete_file(arg1, root);
        } else if (sscanf(command, "delete dir %s", arg1) == 1) {
            delete_directory(arg1, root);
        } else if (strcmp(command, "ls") == 0) {
            list_directory_contents();
        } else if(strcmp(command, "all")==0){
            all_directories(root);
        }else if (sscanf(command, "cd %s", arg1) == 1) {
            change_directory(arg1, root);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else if (sscanf(command, "save %s", arg1) == 1) {
            save_file_system(arg1, root);
        } else if (sscanf(command, "load %s", arg1) == 1) {
            load_file_system(arg1, root);
        } else {
            printf("Unknown command.\n");
        }

    }

}

void all_directories(Folder *root) {
    if (root == NULL) {
        return;
    }

    printf("Contents of folder: %s\n", root->name);

    // List all files in the current folder
    for (int i = 0; i < MAX_FILES; i++) {
        if (root->files[i].name[0] != '\0') {
            printf("  File: %s\n", root->files[i].name);
        }
    }

    // List all child folders and then recursively call for each child folder
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (root->children[i] != NULL) {
            printf("  Folder: %s\n", root->children[i]->name);
        }
    }

    // Recursively list contents of each child folder
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (root->children[i] != NULL) {
            all_directories(root->children[i]);
        }
    }
}

void load_file_system(char arg1[256], Folder* root) {

}

void save_file_system(char arg1[256], Folder* root) {

}

void change_directory(char arg1[256], Folder* root) {

}

void list_directory_contents() {

}

void delete_directory(char arg1[256], Folder* root) {

}

void delete_file(char arg1[256], Folder* root) {

}

void write_file(char arg1[256], char arg2[1024], Folder* root) {

}

void read_file(char arg1[256], Folder* root) {

}

void create_directory(char arg1[256], Folder* root, char path[2000]) {
    Folder* dir = find_folder(path, root);
    if (dir == NULL) {
        return;
    }

    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (dir->children[i] == NULL) {
            Folder* newfolder = (Folder *)malloc(sizeof(Folder));
            if (newfolder == NULL) {
                printf("Memory allocation failed\n");
                return;
            }

            //put / in front of name


            strcpy(newfolder->name, arg1);
            newfolder->number_of_children = 0;
            newfolder->number_of_files = 0;
            newfolder->Parent = dir;
            printf("Name of Parent: %s\n", newfolder->Parent->name);

            for (int j = 0; j < MAX_FILES; j++) {
                newfolder->files[j].name[0] = '\0';
                newfolder->files[j].content[0] = '\0';
                newfolder->files[j].creation_date = 0;
                newfolder->files[j].size = 0;
            }
            for (int j = 0; j < MAX_CHILD_FOLDERS; j++) {
                newfolder->children[j] = NULL;
            }

            dir->children[i] = newfolder;
            dir->number_of_children++;
            printf("Name of child: %s\n", dir->children[i]->name);
            return;
        }
    }
    printf("No available slot for a new directory\n");
}

//return pointer to directory
Folder* find_folder(const char *path, Folder* root)
{
    if (path[0] != '/') {
        printf("Path must be absolute.\n");
        return NULL;
    }

    if (strcmp(path, "/root") == 0)
    {
        return root;
    }

    Folder* dir = root;

    char temp_path[1000];
    strcpy(temp_path, path);

    char* token = strtok(temp_path, "/");
    while (token != NULL) {
        int found = 0;

        //check wehter fist token is root otherwise there are problems with the Children function.
        if (strcmp(token, "root") == 0)
        {
            token = strtok(NULL, "/");
            continue;
        }
        // Search for the token in the current directory's subdirectories
        for (int i = 0; i < dir->number_of_children; i++) {
            if (strcmp(dir->children[i]->name, token) == 0) {
                // Move to the subdirectory if found
                dir = dir->children[i];
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Path was invalid\n");
            return NULL;
        }

        // Get the next token
        token = strtok(NULL, "/");
    }

    return dir;
}

Folder* find_file(const char *path, Folder* root)
{

}

void create_file(char arg1[256], char arg2[1024], Folder* root) {

}
