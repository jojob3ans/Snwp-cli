#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 255

char*
getTimeInfo()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

char*
fileNameStd(char fileName[255])
{
    for (int i = 0; i < strlen(fileName); i++)
    {
        if (fileName[i] == ' ' || fileName[i] == ':')
        {
            fileName[i] = '_';
        }
    }
    return fileName;
}

void
writeToFile
(char path[MAX_LINE_LENGTH], char data[MAX_LINE_LENGTH])
{
    FILE *fp = fopen(path, "a");

    if(!fp) {
        fprintf(stderr, "[!]:\tError: couldn't open file snwp.list '%s'\n\t\t (it's may not existing). Created the new one.\n", "./snwp.list");
        system("touch ~/Projects/snowup-cli/snwp.list");
        exit(1);
    }

    fprintf(fp, "%s", data);
    fclose(fp);
    
}

int
checkForExistence
(char file_path[MAX_LINE_LENGTH], char data[MAX_LINE_LENGTH])
{
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    FILE *fp = fopen("./snwp.list", "r");
    
    if (!fp)
    {
        fprintf(stderr, "[!]:\tError: couldn't open file snwp.list '%s'\n\t\t (it's may not existing). Created the new one.\n", "./snwp.list");
        system("touch ~/Projects/snowup-cli/snwp.list");
        exit(1);
    }

    line_size = getline(&line_buf, &line_buf_size, fp);

    while (line_size >= 0)
    {
        line_count++;
        if (strcmp(data, line_buf) == 0) {
            return 1;
        }

        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    free(line_buf);
    line_buf = NULL;
    fclose(fp);
    return 0;
}


int
addToList
(char path[MAX_LINE_LENGTH])
{
    FILE *pf;
    char command[MAX_LINE_LENGTH];
    char data[MAX_LINE_LENGTH];

    sprintf(command, "realpath %s", path); 
    pf = popen(command,"r");

    fgets(data, 512, pf);

    if (pclose(pf) != 0)
        fprintf(stderr,"[!]:\tError: Failed to close command stream\n");

    int isExistInList = checkForExistence("snwp.list", data);
    if (!isExistInList)
    {
        printf("[+]:\t%s", data);
        writeToFile("./snwp.list", data);
        return 0; 
    }
    else
    {
        printf("[-]:\t%s - Already existing\n", data);
    }
    return 1;
}

int
printList()
{
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    FILE *fp = fopen("./snwp.list", "r");
    
    if (!fp)
    {
        fprintf(stderr, "[!]:\tError: couldn't open file snwp.list '%s'\n\t\t (it's may not existing). Created the new one.\n", "./snwp.list");
        system("touch ~/Projects/snowup-cli/snwp.list");
        exit(1);
    }

    line_size = getline(&line_buf, &line_buf_size, fp);
    
    printf("[^]:\t List of the included paths\n\n");
    while (line_size >= 0)
    {
        line_count++;
        printf("[%02d]:\t%s", line_count, line_buf);
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    printf("\n[$]:\tTotal number of paths: %d\n", line_count);
    free(line_buf);
    line_buf = NULL;
    fclose(fp);

    return line_count;
}

void
assembleBackup()
{
    char commandLine[MAX_LINE_LENGTH] = "tar cf ~/.snwp.backup/";
    char timeinfo[MAX_LINE_LENGTH];
    char fileName[MAX_LINE_LENGTH] = "";

    sscanf(getTimeInfo(), "%[^\n]", timeinfo);
    strcat(fileName, fileNameStd(timeinfo));
    strcat(fileName, ".tar");
    strcat(commandLine, fileName);

    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    FILE *fp = fopen("./snwp.list", "r");
    
    if (!fp)
    {
        fprintf(stderr, "[!]:\tError: couldn't open file snwp.list '%s'\n\t\t (it's may not existing). Created the new one.\n", "./snwp.list");
        system("touch ~/Projects/snowup-cli/snwp.list");
        exit(1);
    }

    line_size = getline(&line_buf, &line_buf_size, fp);

    while (line_size >= 0)
    {
        char path[MAX_LINE_LENGTH];
        char fileOrDir[MAX_LINE_LENGTH];

        sscanf(line_buf, "%[^\n]", path);

        char words[255];
        strcpy(words, path);

        char delim[] = "/";
        char *ptr = strtok(words, delim);

        while(ptr != NULL)
        {
            sscanf(ptr,"%s", fileOrDir);
            // printf("%s\n", word);
            ptr = strtok(NULL, delim);
        }
        
        strcat(commandLine," -C ");
        strcat(path, "/.. ");
        strcat(path, fileOrDir);
        strcat(commandLine, path);
        line_size = getline(&line_buf, &line_buf_size, fp);
    }

    strcat(commandLine, "> /dev/null 2>&1");
    system(commandLine);

    strcpy(commandLine, "pbzip2 -b15 ~/.snwp.backup/");
    strcat(commandLine, fileName);
    system(commandLine);

    printf("[v] Backup %s is created!\n", fileName);

    fclose(fp);
}

void 
clearList()
{
    system("rm snwp.list && touch snwp.list");
}