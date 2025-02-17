#include "../include/minishell.h"

void exit_code(t_minishell *minishell, char **command, char **argv)
{
    pid_t pid = fork();
    int status;

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (execve(*command, argv, minishell->environment) == -1)
        {
            perror("Error runing execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        minishell->last_exit_status = WEXITSTATUS(status);
        printf("%d\n", minishell->last_exit_status);
    }
}