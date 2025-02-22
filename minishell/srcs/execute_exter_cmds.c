#include "../include/minishell.h"

void exec_extercmds(char **argv)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            perror("error with execve");
            exit(1);
        }
    }
    else if (pid < 0)
        perror("error with fork");
    else
        waitpid(pid, &status, 0);
}