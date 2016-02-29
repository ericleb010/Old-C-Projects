#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"

void
printHelp()
{
	/**
	 * Things to specify:
	 * 	-> Max length of a line.
	 *	-> Valid syntax for variable assignment and retrieval.
	 *	-> How to reach this help file.
	 */
}

int
main(int argc, char **argv)
{
		char line[LINE_BUFFER];
		char variableName[VALUE_SIZE]; // Assignment requires '=' and >= 1 char.
		char variableValue[VALUE_SIZE]; // Similar argument.
		int quit, background;
		pid_t id, returnedId;
		int childStatus;
		char old[VALUE_SIZE];
		int valid, newLinePlz, ok, ind, i, j;

	

	printf("\n------------------------------------\nWelcome to the Assignment Shell!\n");
	printf("At any time, you can call \"help\" to read the documentation.\n\n");

	// First read in the command line arguments. (There shouldn't be any!)
	if (argc > 1) {
		// Print help text if this is the case. 
		printHelp();
		exit(0);
	}


	createVariableList();
	quit = 0;
	// Now we can begin taking in lines.
	while (!quit)
	{
		printf("\t>>> ");
	
		// Take in a line from the user...
		if (fgets(line, LINE_BUFFER, stdin) == NULL)
		{
			// Some sort of error occured. Indicate this and exit.
			fprintf(stderr, "ERROR: could not read in command.\n");
			continue; 
		}

		// ...minus the newline. 
		line[strlen(line) - 1] = '\0';	
		
		// Check to see if the user is requesting help.
		if (strcmp(line, "help") == 0 || strcmp(line, "--help") == 0)
		{
			printHelp();
			continue;
		}

		// Check to see if the user is asking to quit.
		if (strcmp(line, "quit") == 0 || strcmp(line, "--quit") == 0 || strcmp(line, "q") == 0)
		{
			quit = 1;
			continue;
		}




		newLinePlz = 0;
		// Check to see if it is an assignment.
		for (i = 0; i < strlen(line); i++) 
		{
			if (line[i] == '=') 
			{
				newLinePlz++;

				// Grab the variable name and its value.
				strncpy(variableName, line, i);
				variableName[i] = '\0';
				strcpy(variableValue, &line[i + 1]);

				// Check the validity of the name.
				if (isalpha(variableName[0]) || variableName[0] == '_') 
				{
					valid = 1;
					for (j = 1; j < strlen(variableName); j++)
					{
						if (!(isalpha(variableName[j]) || isdigit(variableName[j]) || variableName[j] == '_'))
						{
							// Not a valid name.
							fprintf(stderr, "ERROR: %s is not a valid variable name. Must not contain characters other than alphanumerics and '_' characters.\n", variableName);
							valid = 0;
							break;
						}
					}
					
					// If we get here, we have a valid name! Attempt to assign the variable.
					if (valid)
					{
						assignVariable(variableName, variableValue);
						break;
					}
				}

				else 
				{
					// Not a valid name.
					fprintf(stderr, "ERROR: %s is not a valid variable name. Must not lead with characters other than alphabetics and '_' characters.\n", variableName);
				}

				break;					
			}
		}
		// If it was an assignment, we request a new line.
		if (newLinePlz)
			continue;




		// If we get here, it wasn't an assignment. Check for substitutions. 
		for (i = 0; i < strlen(line); i++) 
		{
			if (line[i] == '$')
			{
				variableName[0] = '\0';
				// We found the start character. We should expect a bracket:
				if (line[i + 1] == '{')
				{
					// Now extract every character we find until the '}'.
					ind = -1;
					for (j = i + 2; j < strlen(line); j++)
					{
						// Save the ind because we've completed the name.
						if (j != i + 2 && line[j] == '}')
						{
							ind = j;
							break;
						}

						else if (isalpha(line[j]) || isdigit(line[j]) || line[j] == '_')
							strncat(variableName, &line[j], 1);

						else if (isspace(line[j]))
						{
							// No spaces. Expecting a '}'.
							ind = -1;
							break;
						}
						
						else
						{
							// Not a valid character for a variable name.
							fprintf(stderr, "ERROR: variable name requested is made up of illegal characters. Variables will be alphanumeric and contain '_' characters.\n");
							ind = -2;
							break;
						}
					}

					// Check to see if we escaped the loop properly.
					if (ind < 0)
					{	
						newLinePlz++;
						if (ind == -1)
							// Didn't find a closing bracket.
							fprintf(stderr, "ERROR: expected a '}' within statement.\n");	
						break;
					}

					// Now we substitute, as long as we have enough space (and it exists).
					if (getVariable(variableName) == NULL)
					{
						newLinePlz++;
						break;
					}

					if (strlen(line) + strlen(getVariable(variableName)) < LINE_BUFFER)
					{
						strcpy(old, &line[j + 1]);
						snprintf(&line[i], strlen(getVariable(variableName)) + strlen(&line[j]), "%s%s", getVariable(variableName), old);
					}
				}
				else
				{
					// Invalid character found.
					newLinePlz++;
					fprintf(stderr, "ERROR: expecting a '{', found '%c'.\n", line[i + 1]);
					break;
				}
			}
		}

		// Check to see if we broke from the above code due to error.
		if (newLinePlz)
			continue;

	
		createPipeList();

		// If we get to this point, we assume that the line describes a command to be run.
		// Check to see if the command must be run in the background.
		background = 0;
		if (index(line, '&') != NULL)
		{
			*(index(line, '&') - 1) = '\0';
			background++;
		}

		// Check and count any pipes.
		if (checkForPipes(line) < 0) 
		{
			// If we get in here, something happened. Error already printed.	
			destroyPipeList();
			continue;
		}
		ok = 1;

		// If our list has more than one command, we need pipes.
		if (pipeList->nCommands > 1) 
		{			
			// Run the recursive pipe creator and issue the commands.
			makePipesAndRun(background);
		}

		// Otherwise, we can just run the single command as usual.
		else 
		{
			id = fork();
			if (id < 0)
			{
				// Something bad happened. Break!
				ok = 0;
				fprintf(stderr, "ERROR: could not fork process to run command.\n");
			}

			// Child process.
			if (ok && id == 0)
			{
				// This is the child!
				runCommand(0);
				exit(-1);
			}

			// Parent process.
			else 
			{
				if (!background)
				{
					returnedId = wait(&childStatus);
					printStatus(returnedId, childStatus);
					while (returnedId != id)
					{
						returnedId = wait(&childStatus);
						printStatus(returnedId, childStatus);
					}
				}
			}
		}

		destroyPipeList();
	}

	// If we got to here, the user has decided to quit!	
	destroyVariableList();
	printf("\n");
	exit(0);
}
