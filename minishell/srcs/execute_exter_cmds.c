#include "../include/minishell.h"

void exec_extercmds(char **argv)
{
    pid_t pid;
    int status;
    t_minishell *shell;

    shell = NULL;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            perror("error with execve");
            ft_putstr_fd("Error: command not found\n", 2);
            shell->exit_status = 127;
            exit(1);
        }
    }
    else if (pid < 0)
        perror("error with fork");
    else
        waitpid(pid, &status, 0);
}