#ifndef SHELL_H
#define SHELL_H

#define		LINE_BUFFER          8001 // Max of 8kb (plus NUL and including newline characters).
#define		VALUE_SIZE   	     LINE_BUFFER - 2
#define 	MAX_VARIABLES	     256 // Total variables allowed.


// Project-wide structs:

typedef struct {
	char *command;
	int startIndex;		// Start of command within entire line.
	int endIndex;		// End of command (includes pipe characters)
} Command;

typedef struct {
	Command *commands;
	int nCommands;
} PipeList;

typedef struct {
	char *key;
	char *value;
} Variable;

typedef struct {
	int nVar;
	Variable *variables;
} VariableList;


// Global variables:

VariableList *varList;
PipeList *pipeList;


// Prototypes:

void createVariableList();
void destroyVariableList();
void assignVariable(char *key, char *value);
char *getVariable(char *key);

void createPipeList();
void destroyPipeList();
int checkForPipes(char *line);
int printStatus(int returnedId, int childStatus);
int makePipesAndRun(int background);
int runCommand(int listIndex);

#endif
