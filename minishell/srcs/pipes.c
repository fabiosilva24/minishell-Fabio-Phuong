#include "../include/minishell.h"

static void child_process(t_token *tokens, t_minishell *shell, int fd[2])
{
    if (pipe(fd) == - 1)
    {
        printf("Error opening the pipes\n");
        return (1);
    }
}