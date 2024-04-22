#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#define NUM_CHARS 256
#define MAX_FILEPATH_LENGTH 512
#define DICT_FILE "/usr/share/dict/words"

typedef struct trienode
{
    struct trienode *children[NUM_CHARS];
    bool terminal;
}trienode;


trienode *createnode()
{
    trienode *newnode = malloc(sizeof(*newnode));

    for(int i = 0; i < NUM_CHARS; i++)
    {
        newnode->children[i] = NULL;
    }
    newnode->terminal = false;

    return newnode;
}

bool trieinsert(trienode **root, char *signedtext)
{
    if(*root == NULL)
    {
        *root = createnode();
    }

    unsigned char *text = (unsigned char *)signedtext;
    trienode *temp = *root;
    int length = strlen(signedtext);

    for(int i = 0; i < length; i++)
    {
        if(temp->children[text[i]] == NULL)
        {
            temp->children[text[i]] = createnode();
        }
        temp = temp->children[text[i]];
    }

    if(temp->terminal)
    {
        return false;
    }
    else
    {
        temp->terminal = true;
        return true;
    }

}

void printtrie_rec(trienode *node, unsigned char *prefix, int length)
{
        unsigned char newprefix[length+2];
        memcpy(newprefix, prefix, length);
        newprefix[length+1] = 0;

        if(node->terminal)
        {
                printf("%s\n", prefix);

        }

        for(int i = 0; i < NUM_CHARS; i++)
        {
                if(node->children[i] != NULL)
                {
                        newprefix[length] = i;
                        printtrie_rec(node->children[i], newprefix, length+1);
                }
        }
}

void printtrie(trienode *root)
{
        if(root == NULL)
        {
                printf("TREE EMPTY!\n");
                return;
        }
        printtrie_rec(root, NULL, 0);
}

bool searchtrie(trienode *root, char *signedtext)
{
    unsigned char *text = (unsigned char *)signedtext;
    int length = strlen(signedtext);
    trienode *tmp = root;

    for(int i = 0; i < length; i++)
    {
        if(tmp->children[text[i]] == NULL)
        {
            return false;
        }

        tmp = tmp->children[text[i]];
    }

    return tmp->terminal;
}

void freetrie(trienode *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < NUM_CHARS; i++) {
        freetrie(node->children[i]);
    }
    free(node);
}


int istxt(const char *filename) //checks if file is a txt file or not
{
    struct stat sbuf;

    if (stat(filename, &sbuf) == 0) {
        const char *extension = ".txt";
        int length = strlen(filename);
        int extLength = strlen(extension);

        if (length >= extLength && strcmp(filename + length - extLength, extension) == 0) {
            return 1;
        } else {
            return 0;
        }
    }
    else
    {
        perror("stat");
        return 0;
    }

}



int read_file(const char* filepath, trienode *root) { // reads txt file and checks for errors
    struct stat sbuf;
    if (stat(filepath, &sbuf) != 0) {
        perror("stat");
        return EXIT_FAILURE;
    }

    int buffersize = sbuf.st_size + 1;
    char file[MAX_FILEPATH_LENGTH];
    strcpy(file, filepath);

    int fd = open(file, O_RDONLY, 0401);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    int line = 1;
    int column = 1;
    int r;
    char buffer;
    int i = 0;
    char word[50] = "";
    int colofword = 1;
    char delimiters[] = " \t\n,.!?";

    while ((r = read(fd, &buffer, 1)) > 0)
    {

        if(isalpha(buffer) || buffer == '\'')
        {
            word[i++] = buffer;
            
            if (i == 1) {
                colofword = column;
            }
            column++;
        }
        else
        {
            if(isspace(buffer)) {
                column++;
            }
            if(i > 0)
            {
                word[i] = '\0';
                

                
                if(searchtrie(root, word) == 0)
                {
                        printf("%s (%d,%d): %s\n", file, line, colofword, word);
                       
                    }

                    
                
                i = 0;
            }
            if(buffer == '\n')
            {
                line++;
                column = 1;
            }
        }   
    
    }

    if(i > 0)
    {
        word[i] = '\0';
    }


    close(fd);

    return EXIT_SUCCESS;
}

int traverse_directory(const char* filepath, trienode *root) //traverses directory
{
    DIR *dir = opendir(filepath);
    if(dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }
    struct dirent *entry;
    struct stat sbuf;
    char file[256];
    strcpy(file, filepath);
    char fullfilepath[MAX_FILEPATH_LENGTH];
    while ((entry = readdir(dir)) != NULL)spchk.c
    {
        // Process the directory entry
        snprintf(fullfilepath, MAX_FILEPATH_LENGTH, "%s/%s", file, entry->d_name);

        // Populate sbuf with information about the file
        if (stat(fullfilepath, &sbuf) != 0)
        {
            perror("stat");
            continue; // Skip to next entry if stat fails
        }

        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if(S_ISDIR(sbuf.st_mode))
        {
            traverse_directory(fullfilepath, root);
        }

        if (istxt(fullfilepath) == 1) {
            read_file(fullfilepath, root);
        }
    }

    closedir(dir);

    return EXIT_SUCCESS;

}

int main(int argc, char *argv[])
{
        // if (argc < 2) {
        // fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        // return EXIT_FAILURE;
   //}

    struct stat sbuf;
    trienode *root = NULL;
    if (stat(argv[1], &sbuf) != 0) 
        {
            perror("stat");
            return EXIT_FAILURE;
        }
    int buffersize = sbuf.st_size + 1;
    char file[256];
    strcpy(file, argv[1]); 
    char *buffer = (char *)malloc(buffersize);
    int fd = open(file, O_RDONLY, 0401);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    int r;
    while ((r = read(fd, buffer, buffersize)) > 0)
    {
        buffer[r] = '\0';

        char *token = strtok(buffer, " .,?\t\n");


        while(token != NULL)
        {
            trieinsert(&root, token);
            token[0] = toupper(token[0]);
            trieinsert(&root, token);
            for(int i = 1; i < strlen(token); i++){ 
                token[i] = toupper(token[i]);
            }
            trieinsert(&root, token);
            token = strtok(NULL, " .,?\t\n");
        }

    }

    free(buffer);

    close(fd);

    for(int i=2; i < argc; i++){
        if(istxt(argv[i])){
            read_file(argv[i], root);
        } else {
    traverse_directory(argv[i], root); 
        }
    }

    freetrie(root);


    return EXIT_SUCCESS;

}
