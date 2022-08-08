#include "shell.h"

/**
 * main - Simple Shell Program
 * @ac: number of command line arguments
 * @av: array of arguments provided
 * @env: unix environmental variables
 *
 * Return: Always 0
 */

int main(int ac, char **av, char **env)
{
	size_t buffer_size = 0;
	char **command, *command_path, *buffer = NULL;
	hshpack *shpack;
	int error_num = 0, exit_num = 0, relation = 0, run_able = 0, size_env = 0, enul = 0;
	ssize_t isBuiltIn;

	if (ac > 1 || av == NULL)
		write(2, "Please you need to provide some arguments\n", 42), exit(127);

	signal(SIGINT, signal_handler);
