#include "shell.h"

/**
 * _dplength - Checks the length of a given double pointer (ending in null)
 * @str: given double pointer
 *
 * Return: Length of the double pointer
 *
 */

int _dplength(char **str)
{
	int i = 0;

	if (!str)
		return (0);

	while (str[i] != NULL)
