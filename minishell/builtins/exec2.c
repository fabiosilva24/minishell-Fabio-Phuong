#include "../include/minishell.h"

static int count_args(char **args)
{
    int i;

    i = 0;
    while (args[i])
        i++;
    return (i);
}
static int handle_builtin_part1(t_cmd *cmd, t_minishell *shell)
{
    int token_count;

    token_count = count_args(cmd->args);

    if (ft_strcmp(cmd->args[0], "cd") == 0)
    {
        builtin_cd(cmd->args, &shell->envp);
        return (shell->status);
    }
    if (ft_strcmp(cmd->args[0], "echo") == 0)
    {
        my_echo(token_count, cmd->args);
        return (shell->status);
    }
    if (ft_strcmp(cmd->args[0], "exit") == 0)
    {
        builtin_exit(cmd, shell, 1);
        return (shell->status);
    }
    if (ft_strcmp(cmd->args[0], "env") == 0)
    {
        builtin_env(shell->envp, &shell->status);
        return (shell->status);
    }
    return (-1);
}

static int handle_builtin_part2(t_cmd *cmd, t_minishell *shell)
{
    if (ft_strcmp(cmd->args[0], "pwd") == 0)
    {
        builtin_pwd(&shell->status);
        return (shell->status);
    }
    if (ft_strcmp(cmd->args[0], "export") == 0)
    {
        shell->envp = builtin_export(cmd->args, shell->envp);
        return (shell->status);
    }
    if (ft_strcmp(cmd->args[0], "unset") == 0)
    {
        shell->envp = builtin_unset(cmd->args, shell->envp, &shell->status);
        return (shell->status);
    }
    return (-1);
}

int handle_builtin(t_cmd *cmd, t_minishell *shell)
{
    int ret;

    if (!cmd || !cmd->args[0])
        return (0);
    ret = handle_builtin_part1(cmd, shell);
    if (ret != -1)
        return (ret);
    ret = handle_builtin_part2(cmd, shell);
    if (ret != -1)
        return (ret);
    return (0);
}

void handle_exit_status(int status, t_minishell *shell)
{
    if (!shell)
        return;
    if (status >= 0 && status <= 255)
        shell->exit_status = status;
    else
        shell->exit_status = 128 + (status % 256);
}
