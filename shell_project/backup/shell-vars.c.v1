#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"


// Function to instantiate the variable list.
void
createVariableList() 
{
		int i;

	varList = (VariableList *) malloc(sizeof(VariableList));
	assert(varList != NULL);

	varList->variables = malloc(sizeof(Variable) * MAX_VARIABLES);
	assert(varList->variables != NULL);
	varList->nVar = 0;

	for (i = 0; i < MAX_VARIABLES; i++)
	{
		varList->variables[i].key = malloc(VALUE_SIZE);
		varList->variables[i].value = malloc(VALUE_SIZE);
	}
}

// Frees the list.
void	
destroyVariableList()
{
		int i;

	for (i = 0; i < MAX_VARIABLES; i++)
	{
		free(varList->variables[i].key);
		free(varList->variables[i].value);
	}

	free(varList->variables);
	free(varList);
}

// Function to assign a variable. Also creates the list if that was not done yet.
void
assignVariable(char *key, char *value)
{
		int i;

	// Find the variable we're looking for.
	for (i = 0; i < varList->nVar; i++)
	{
		if (strcmp(key, varList->variables[i].key) == 0)
		{
			strcpy(varList->variables[i].value, value);
			return;
		}
	}

	// If we get here, we didn't find a variable.
	if (varList->nVar < MAX_VARIABLES)
	{
		strcpy(varList->variables[varList->nVar].key, key);
		strcpy(varList->variables[varList->nVar].value, value);
		varList->nVar++;
		return;
	}

	// And if we get here, we've reached the maximum allowed variables.
	fprintf(stderr, "ERROR: already holding the maximum %d variables.\n", MAX_VARIABLES);
}

char *
getVariable(char *key)
{
		int i;

	// First check to see if there is a list.
	if (varList == NULL)
	{
		fprintf(stderr, "ERROR: Cannot retrieve %s, there are no variables.\n", key);
		return(NULL);
	}

	// Find the variable we are looking for.
	for (i = 0; i < varList->nVar; i++)
	{
		if (strcmp(key, varList->variables[i].key) == 0)
			return(varList->variables[i].value);
	}

	// We fall here if we don't find anything.
	fprintf(stderr, "ERROR: Cannot retrieve %s, it has not been declared.\n", key);
	return(NULL);
}

