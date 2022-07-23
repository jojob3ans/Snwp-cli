#ifndef COMS_H
#define COMS_H

void        writeToFile(char path[255], char data[255]);
int         checkForExistence(char file_path[255], char data[255]);
char*       getTimeInfo();
char*       fileNameStd(char fileName[255]);

int         addToList(char path[255]);
void        assembleBackup();
int         printList();
void        clearList();

#endif