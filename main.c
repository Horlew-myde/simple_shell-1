#include "main.h"

void sig_handler(int sig);
int execute(char **args, char **front);
/**
 * sig_handler - Prints a new prompt upon a signal.
 * @sig: The signal.
 */
void sig_handler(int sig)
{
    (void)sig;
    write(STDIN_FILENO, "\n$ ", 3);
}


/**
* execute - Executes a command in a child process.
* @args: An array of arguments.
* @front: A double pointer to the beginning of args.
*
* Return: If an error occurs - a corresponding error code.
*         O/w - The exit value of the last executed command.
*/
int execute(char **args, char **front)
{
    pid_t child_pid;
    int status, flag = 0, ret = 0;
    char *command = args[0];

    if (command[0] != '/' && command[0] != '.')
    {
        flag = 1;  /* indicating that the command is neither a directory nor a hidden file */
        command = get_location(command);
    }

    if (!command || (access(command, F_OK) == -1))
    {
        if (errno == EACCES)
            ret = (create_error(args, 126));
        else
            ret = (create_error(args, 127));
    }
    else
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            if (flag)
            free(command);
            perror("Error child:");
            return (1);
        }
    
        if (child_pid == 0)
        {
            execve(command, args, environ);
            if (errno == EACCES)
                ret = (create_error(args, 126));
            free_env();
            free_args(args, front);
            free_alias_list(aliases);
            _exit(ret);
        }
        else
        {
            wait(&status);
            ret = WEXITSTATUS(status);
        }
    }
    
    if (flag)
        free(command);
    return (ret);
}

/**
 * main - A simple shell (Unix command interpreter)
 * @argc: The number of arguments supplied
 * @argv: An array of arguments supplied
 *
 * Return: The return value of the last executed command
 * 
 * Description: 
 * Establish the name of the program
 * set history to 1
 * set aliases to NULL
 * Listen for signals and set a signal handler function to handle each signal that will be received
 * set return value to 0
 * create a copy of the environmental variables
 * Check if program was started with command line arguments and process those arguments
 * check if a terminal is being used
 * start the infinite loop (for Prompt, Read, Parse, Execute)
 */

 int main(int argc, char **argv)
 {
    int return_code = 0, return_value;
    int *execute_return = &return_value;
    
    shell = argv[0];
    history_count = 1;
    aliases = NULL;

    signal(SIGINT, sig_handler);

    return_value = 0;
    environ = _copyenv();
    if (!environ)
        exit(-100);

    if (argc != 1)
    {
        return_code = proc_file_commands(argv[1], execute_return);
        free_env();
        free_alias_list(aliases);
        return (*execute_return);
    }

    if (!isatty(STDERR_FILENO))
    {
        while (return_code != END_OF_FILE && return_code != EXIT)
        return_code = handle_args(execute_return);
        free_env();
        free_alias_list(aliases);
        return (*execute_return);
    }

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        return_code = handle_args(execute_return);
        if (return_code == END_OF_FILE || return_code == EXIT)
        {
            write(STDERR_FILENO, "\n", 1);
            free_env();
            free_alias_list(aliases);
            exit(*execute_return);
        }
    }
    free_env();
    free_alias_list(aliases);
    return (*execute_return);
 }