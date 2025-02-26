#include "../include/minishell.h"

/*void handle_dollar_questionmark(char **arg, t_minishell *shell)
{
    char *status_str;
    int len;

    status_str = ft_itoa(shell->exit_status);
    if (!status_str)
        return;

    len = strlen(status_str);
    memmove(*arg + len, *arg + 2, strlen(*arg) - 1); // Shift the rest of the string
    memcpy(*arg, status_str, len); // Replace $? with the exit status

    free(status_str);
}*/