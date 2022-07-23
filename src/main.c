#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coms.h"

void	initCommand(int argc, char *argv[]);

int
main 
(int argc, char *argv[])
{
	initCommand(argc, argv);
	return 0;
}

void
initCommand
(int argc, char *argv[])
{
	if (argc == 1 || (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ))
	{
		system("cat ~/Projects/snwp-cli/help");
	}
	else if (strcmp(argv[1], "--add-to-list") == 0)
	{
		if (argc < 3) {
			fprintf(stderr, "[!]:\tError: Empty arguments for --add-to-list\n");
		} else {
			for (int i = 2; i < argc; i++) 
			{
				addToList(argv[i]);
			}
		}
	} 
	else if (strcmp(argv[1], "--assemble") == 0 || strcmp(argv[1], "-a") == 0)
	{
		assembleBackup();
	} 
	else if (strcmp(argv[1], "--show-list") == 0 || strcmp(argv[1], "-l") == 0) 
	{
		printList();
	} 
	else if (strcmp(argv[1], "--clear-list") == 0 || strcmp(argv[1], "-c") == 0)
	{
		clearList();
	}
	else {
		fprintf(stderr, "[!]:\tError: you are using wrong commands\n\tEnter the \' ./snwp --help \' to see available commands\n");
	}
}
