#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

#include "shell.h"


// Function to instantiate the list of commands and pipes. This only accounts for one command.
void
createPipeList()
{	
	pipeList = (PipeList *) malloc(sizeof(PipeList));
	assert(pipeList != NULL);

	pipeList->nCommands = 0;
	pipeList->commands = (Command *) malloc(sizeof(Command));
	assert(pipeList->commands != NULL);

	pipeList->commands->command = (char *) malloc(LINE_BUFFER);
}


// Function to free all allocated memory in heap.
void
destroyPipeList()
{
		int i;

	for (i = 0; i < pipeList->nCommands; i++)
		free(pipeList->commands[i].command);

	free(pipeList->commands);
	free(pipeList);
}


// Function to add a number of commands to the queue.
int
reallocPipeList()
{
		int oldN;
		Command *list;

	// Save old amount of space.
	oldN = pipeList->nCommands;

	// Re-allocate the memory to reflect the request.
	list = (Command *) realloc(pipeList->commands, (oldN + 1) * sizeof(Command));
	
	// If this returned null, there must have been an error.
	if (list == NULL)
	{
		fprintf(stderr, "ERROR: reallocation of memory for commands failed.\n");
		return -1;
	}

	list[oldN].command = (char *) malloc(LINE_BUFFER);

	// Otherwise, let's actually update our list to reflect the change.
	pipeList->commands = list;

	return 0;
}


// Takes in a line, scans for pipe requests, and stores each individual command.
int
checkForPipes(char *line)
{
		int index, i;

	// For every '|' character we find, we will have another command to distinguish.
	i = 0;
	pipeList->nCommands = 0;
	while (i < strlen(line))
	{	
		// Find the next character.
		while (line[i] == ' ')
			i++;
		
		// If we find a pipe, throw an error; we expected a command!
		if (line[i] == '|')
		{
			// We expect that there would be a command, not another pipe.
			fprintf(stderr, "ERROR: expected a command at char %d.\n", i);
			return -1;
		}

		index = i;

		// Find the next pipe (or go to end of line).
		while (line[i] != '|' && i < strlen(line) + 1)
			i++;
	
		// Copy the command into the command list.
		strncpy(pipeList->commands[pipeList->nCommands].command, &line[index], i - index - 1);
		pipeList->commands[pipeList->nCommands].command[i - index - 1] = '\0';
		pipeList->nCommands++;
		i++;
		i++;
		
		// Make room for another command.
		if (reallocPipeList() < 0)
			// Something happened. Error has already been printed.
			return -1;
	}

	return 0;
}

// Method for the WIFEXITED macro.

int
printStatus(returnedId, childStatus)
{

	int id = (in != stdin) ? 0 : returnedId;
	int status = (in != stdin) ? 0 : childStatus;

	// Check status of child.	
	if (WIFEXITED(childStatus))
	{
		// Child exited OK!
		if (childStatus != 0)
		{	
			fprintf(stderr, "Child (<%d>) exited -- failure (Status: %d)\n", id, status);
			return -1;
		}
		else			
			fprintf(stderr, "Child (<%d>) exited -- success (Status: %d)\n", id, status);
	}

	else 
	{
		// Child didn't exit properly.		
		fprintf(stderr, "Child (<%d>) crashed!\n", id);
		return -1;
	}

	return 0;
}


// Main method for calling UNIX commands.

int
runCommand(int listIndex)
{
		int index, space, i, j; 
		int pieces = 1;
		char **argv;

	space = 0;
	// First break up the command into pieces.

	for (i = 0; i < strlen(pipeList->commands[listIndex].command); i++)
	{
		// Check to see if there is another possible piece.
		if (pipeList->commands[listIndex].command[i] == ' ')
			space = 1;
		
		// If we enter here, then we've detected more chars after the space.
		else if (space == 1)
		{
			pieces++;
			space = 0;
		}
	}

	// Now "pieces" represents how much storage we need. (Also need nul.)
	argv = (char **) malloc(sizeof(char *) * (pieces + 1));
	
	index = 0;
	j = 0;
	
	// If we actually found spaces, we have to extract each piece.
	if (pieces > 1)
	{
		i = 0;
		// Run the loop again, this time we know what is a separator.
		while (1)
		{
			index = i;
				
			if (pieces == 1)
				i = strlen(pipeList->commands[listIndex].command) - 1;

			else 
			{
				// Find the next whitespace.
				while (pipeList->commands[listIndex].command[i] != ' ')
					i++;
				i--;
			}

			// Insert piece we've found.
			argv[j] = (char *) malloc(LINE_BUFFER);
			
			if (pieces == 1)
			{
				// Grab rest of string and break.
				strcpy(argv[j], &pipeList->commands[listIndex].command[index]);
				j++;
				break;
			}
	
			strncpy(argv[j], &pipeList->commands[listIndex].command[index], i - index + 1);
			argv[j][i - index + 1] = '\0';
			j++;
			pieces--;

			// Otherwise, move back into whitespace and skip it all.		
			i++;
			while (pipeList->commands[listIndex].command[i] == ' ')
				i++;
		}
	}

		
	// If we didn't find spaces, the whole command is the command name.
	else
	{
		argv[0] = (char *) malloc(LINE_BUFFER);
		strcpy(argv[0], pipeList->commands[listIndex].command);
		j++;	
	}

	// Null-terminate our array.
	argv[j] = NULL;

	// Now we have the command name and the rest stored in an array of pointers.
	execvp(argv[0], argv);

	// If we get here, something went wrong.
	fprintf(stderr, "ERROR: Could not run %s.\n", argv[0]);
	exit(-1);
}


// Recursive method for running commands through pipes.
int
makePipesAndRun(int background)
{
		int childStatus;
		int pipes[2];
		int i, j, ok, index, nextRead, lowest;

		pid_t id[pipeList->nCommands], returnedId;

	// Find the lowest file descriptor.
	lowest = dup(0);
	close(lowest);

	// Initialisation.
	nextRead = -1;
	index = 0;
	i = 0;

	// Go through our commands.
	while (index != pipeList->nCommands)
	{

		// Create a set of pipes.
		if (pipe(pipes) < 0)
			return -1;

		// Fork a child process and save its pid.
		id[index] = fork();
		if (id[index] < 0)
		{
			fprintf(stderr, "ERROR: could not fork process to run command.\n");
			return -1;
		}

		// Code for the child process:
		if (id[index] == 0)
		{			
			// If this is the first command, don't close stdin. 
			if (nextRead != -1)
			{
				// Otherwise, replace stdin with read side of last pipe.
				close(0);
				dup(nextRead);
			}

			// If this is the last command, don't close stdout.
			if (index != pipeList->nCommands - 1)
			{	
				// Otherwise, replace stdout with write side of new pipe.
				close(1);
				dup(pipes[1]);
			}
	
			// Iterate through all open file descriptors our parent has opened so far and close them.
			for (i = 0; i <= index; i++)
			{
				close(lowest * i + 3);
				close(lowest * i + 3 + 1);
			}
				
			// Execute the command.
			runCommand(index);

			exit(-1);
		}

		// Parent code! Store the read fd for the next child process and move forward.
		nextRead = pipes[0];
		index++;
	}

	// Close all file descriptors we've opened. 
	// Now the only processes that have them open are those that need them.
	for (i = 0; i < pipeList->nCommands; i++)
	{
		close(lowest + 2 * i);
		close(lowest + 1 + 2 * i); 
	}

	ok = 0;
	// For all of our processes:
	for (i = 0; i < pipeList->nCommands; i++)
	{
		// If the user requested this in the background, we skip the wait().
		if (!background)
		{
			// Wait for child process to finish and print result.
			returnedId = wait(&childStatus);
			if (printStatus(returnedId, childStatus) < 0)
				ok = -1;

			// If the child we find was not what we expected, we found a background process.
			// As long as we don't find our child, we keep waiting.
			while (returnedId != id[i])
			{
				index = 0;
				// First, check if it matches any of the other processes.
				for (j = 0; j < pipeList->nCommands; j++)
				{
					if (returnedId == id[j])
						// All good!
						index = 1;
				}
				if (index == 1)
					break;

				// Otherwise, we wait for ours.
				returnedId = wait(&childStatus);
				if (printStatus(returnedId, childStatus) < 0)
					ok = -1;
			}
		}
	}	
	return ok;
}
