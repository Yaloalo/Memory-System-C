#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 255
#define MAX_FILES 100
#define MAX_CONTENT_LENGTH 1024
#define MAX_CHILD_FOLDERS 100
//TODO When Linux is here save file content in txt file also add a folder system and a load and save function.
// TODO so that the system can be used when booting to load the file system up. Inoed would also be nice.
struct Folder;

typedef struct File
{
    char name[MAX_NAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    size_t size;
    time_t creation_date;
    struct Folder* parent_folder;
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
void get_input(char* buffer, size_t buffer_size);
void create_file(char arg1[256], char arg2[1024], Folder* folder, char path[1000]);
void create_directory(char arg1[256], Folder* folder, char path[2000]);
void read_file(char arg1[256], Folder* folder);
void write_file(char path[256], Folder* root);
void delete_file(char arg1[256], Folder* folder);
void delete_directory(char arg1[256], Folder* folder);
void list_directory_contents(Folder* current_folder);
Folder* change_directory(char arg1[256], Folder* folder);
Folder* find_folder(const char *path, Folder* root);
File* find_file(const char *path, Folder* root);
void all_directories(Folder *pFolder);
void save_file_to_disk(File* file);
void delete_file_from_disk(char* filename);

int mainn()
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
    rootfolder->number_of_files = 0;
    for (int i = 0; i < MAX_FILES; i++)
    {
        rootfolder->files[i].name[0] = '\0';
        rootfolder->files[i].content[0] = '\0';
        rootfolder->files[i].creation_date =  0;
        rootfolder->files[i].size = 0;
    }
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++)
    {
        rootfolder->children[i] = NULL;
    }
    return rootfolder;
}

void handle_command(Folder* root)
{
    char command[256];
    char name[256];
    char content[1024];
    char path[2000];

    // Saves current folder
    Folder* current_folder = root;

    while (1)
    {
        printf(">");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (sscanf(command, "create file %255s %1023s %1999s", name, content, path) == 3) {
            create_file(name, content, root, path);
        } else if (sscanf(command, "create dir %255s %1999s", name, path) == 2) {
            create_directory(name, root, path);
        } else if (sscanf(command, "read %1999s", path) == 1) {
            read_file(path, root);
        } else if (sscanf(command, "write %1999s", path) == 1) {
            write_file(path, root);
        } else if (sscanf(command, "delete file %255s", name) == 1) {
            delete_file(name, root);
        } else if (sscanf(command, "delete dir %255s", name) == 1) {
            delete_directory(name, root);
        } else if (strcmp(command, "ls") == 0) {
            list_directory_contents(current_folder);
        } else if(strcmp(command, "all") == 0){
            all_directories(root);
        } else if (sscanf(command, "cd %1999s", path) == 1) {
            current_folder = change_directory(path, root);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else if (sscanf(command, "save %255s", name) == 1) {
            // Placeholder for save_file_system(name, root);
        } else if (sscanf(command, "load %255s", name) == 1) {
            // Placeholder for load_file_system(name, root);
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

    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (root->children[i] != NULL) {
            all_directories(root->children[i]);
        }
    }
}

Folder* change_directory(char path[256], Folder* root)
{
    Folder* dir = find_folder(path, root);
    if (dir != NULL) {
        printf("Current directory: %s\n", dir->name);
    } else {
        printf("Directory not found.\n");
    }
    return dir;
}

void list_directory_contents(Folder* current_folder) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (current_folder->files[i].name[0] != '\0') {
            printf("  File: %s\n", current_folder->files[i].name);
        }
    }

    // List all child folders
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (current_folder->children[i] != NULL) {
            printf("  Folder: %s\n", current_folder->children[i]->name);
        }
    }
}

void get_input(char* buffer, size_t buffer_size) {
    printf("Enter input: ");
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        // Remove the newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        // Handle error or end of file condition
        printf("Error reading input.\n");
        buffer[0] = '\0'; // Set buffer to an empty string in case of error
    }
}

void delete_directory(char path[256], Folder* root) {
    Folder* folder = find_folder(path, root);
    if (folder == NULL) {
        printf("Directory not found.\n");
        return;
    }

    Folder* parent_folder = folder->Parent;
    if (parent_folder == NULL) {
        printf("Cannot delete root directory.\n");
        return;
    }

    // Reassign children to parent folder
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (folder->children[i] != NULL) {
            parent_folder->children[parent_folder->number_of_children++] = folder->children[i];
            folder->children[i]->Parent = parent_folder;
        }
    }

    // Reassign files to parent folder
    for (int i = 0; i < MAX_FILES; i++) {
        if (folder->files[i].name[0] != '\0') {
            parent_folder->files[parent_folder->number_of_files++] = folder->files[i];
            folder->files[i].parent_folder = parent_folder;
        }
    }

    // Remove the folder from parent's children list
    for (int i = 0; i < MAX_CHILD_FOLDERS; i++) {
        if (parent_folder->children[i] == folder) {
            parent_folder->children[i] = NULL;
            parent_folder->number_of_children--;
            break;
        }
    }

    free(folder);
}

void delete_file(char path[256], Folder* root) {
    File* file = find_file(path, root);
    if (file == NULL) {
        printf("File not found.\n");
        return;
    }

    delete_file_from_disk(file->name);

    file->name[0] = '\0';
    file->content[0] = '\0';
    file->creation_date =  0;
    file->size = 0;
}

void write_file(char path[256], Folder* root) {
    File* file = find_file(path, root);
    if (file == NULL) {
        printf("File not found.\n");
        return;
    }

    printf("File name: %s\n File content: %s\n", file->name, file->content);
    char input[1000];
    get_input(input, sizeof(input));
    strcpy(file->content, input);

    save_file_to_disk(file);
}

void read_file(char path[256], Folder* root) {
    File* file = find_file(path, root);
    if (file == NULL) {
        printf("File not found.\n");
        return;
    }
    printf("File name: %s\n File content: %s\n", file->name, file->content);
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

            strcpy(newfolder->name, arg1);
            newfolder->number_of_children = 0;
            newfolder->number_of_files = 0;
            newfolder->Parent = dir;

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
            return;
        }
    }
    printf("No available slot for a new directory\n");
}

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

        if (strcmp(token, "root") == 0)
        {
            token = strtok(NULL, "/");
            continue;
        }

        for (int i = 0; i < dir->number_of_children; i++) {
            if (strcmp(dir->children[i]->name, token) == 0) {
                dir = dir->children[i];
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Path was invalid\n");
            return NULL;
        }

        token = strtok(NULL, "/");
    }

    return dir;
}

File* find_file(const char *path, Folder* root) {
    if (path[0] != '/') {
        printf("Path must be absolute.\n");
        return NULL;
    }

    Folder* dir = root;
    char temp_path[1000];
    strcpy(temp_path, path);

    char* token = strtok(temp_path, "/");
    char* file_name = NULL;

    while (token != NULL) {
        int found = 0;

        if (strcmp(token, "root") == 0) {
            token = strtok(NULL, "/");
            continue;
        }

        for (int i = 0; i < dir->number_of_children; i++) {
            if (strcmp(dir->children[i]->name, token) == 0) {
                dir = dir->children[i];
                found = 1;
                break;
            }
        }

        if (!found) {
            file_name = token;
            break;
        }

        token = strtok(NULL, "/");
    }

    if (file_name == NULL) {
        printf("Path was invalid or did not specify a file.\n");
        return NULL;
    }

    for (int i = 0; i < dir->number_of_files; i++) {
        if (strcmp(dir->files[i].name, file_name) == 0) {
            return &dir->files[i];
        }
    }

    printf("File not found in the specified path.\n");
    return NULL;
}

void create_file(char name[256], char content[1024], Folder* root, char path[1000]) {
    Folder* dir = find_folder(path, root);
    if (dir == NULL || dir->number_of_files >= MAX_FILES) {
        printf("Cannot add file to the specified directory.\n");
        return;
    }

    File* file = &dir->files[dir->number_of_files++];
    strcpy(file->name, name);
    strcpy(file->content, content);
    file->creation_date = time(NULL);
    file->parent_folder = dir;
    file->size = strlen(file->content);

    save_file_to_disk(file);

    printf("File name: %s\n", file->name);
    printf("Content: %s\n", file->content);
    printf("Size: %zu bytes\n", file->size);
    printf("Creation date: %s", ctime(&file->creation_date));
}

void save_file_to_disk(File* file) {
    char filename[512];
    snprintf(filename, sizeof(filename), "%s.txt", file->name);

    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(f, "%s", file->content);
    fclose(f);
}

void delete_file_from_disk(char* filename) {
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s.txt", filename);
    if (remove(filepath) == 0) {
        printf("Deleted file: %s\n", filepath);
    } else {
        printf("Unable to delete file: %s\n", filepath);
    }
}
