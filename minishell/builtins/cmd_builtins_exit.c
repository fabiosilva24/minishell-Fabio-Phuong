
#include "../include/minishell.h"

static int valid_exit_code(const char *arg)
{
    int     i;

    if (!arg || !arg[0] || ((arg[0] == '-' || arg[0] == '+') && !arg[1]))
        return (0);
    i = (arg[0] == '-' || arg[0] == '+');
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            return (0);
        i++;
    }
    return (1);
}


static void handle_exit_with_arg(t_cmd *cmd, t_minishell *shell, int should_exit)
{
    long exit_code;

    if (!valid_exit_code(cmd->args[1]))
    {
        printf("%s", EXIT_NUM_ERROR);
        shell->exit_status = 255;
        exit(255);
    }
    exit_code = ft_atol(cmd->args[1]);
    shell->exit_status = (unsigned char)exit_code;
    if (should_exit)
        exit(shell->exit_status);
}

void builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit)
{
    if (!cmd)
    {
        shell->status = 1;
        return;
    }
    if (should_exit && !(cmd->args[1] && cmd->args[2]))
        printf("%s", EXIT_MSG);
    if (cmd->args[1] && cmd->args[2])
    {
        printf("%s", EXIT_MANY_ARGS);
        shell->status = 1;
        return;
    }
    if (cmd->args[1])
        handle_exit_with_arg(cmd, shell, should_exit);
    else
    {
        shell->exit_status = 0;
        if (should_exit)
            exit(0);
    }
}
