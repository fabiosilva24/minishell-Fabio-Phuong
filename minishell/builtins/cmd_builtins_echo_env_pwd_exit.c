
#include "../include/minishell.h"

void builtin_echo(t_cmd *cmd, int *status)
{
    int i;

    i = 1;
    *status = 0;
    if (!cmd->args[1])
    {
        printf("\n");
        return;
    }
    while (cmd->args[i] && !ft_strncmp(cmd->args[i], "-n",
            ft_strlen(cmd->args[i])))
        i++;
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (ft_strncmp(cmd->args[1], "-n", ft_strlen(cmd->args[1])))
        printf("\n");
}

void builtin_env(char **envp, int *status)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (!ft_strchr(envp[i], '='))
            i++;
        else
        {
            printf("%s\n", envp[i]);
            i++;
        }
    }
    *status = 0;
}

void builtin_pwd(int *status)
{
    char cwd[4096];

    getcwd(cwd, 4097);
    printf("%s\n", cwd);
    *status = 0;
}

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

void builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit)
{
    long    exit_code;

    if (!cmd)
    {
        shell->status = 1;
        return;
    }
    if (should_exit && !(cmd->args[1] && cmd->args[2]))
        write(1, EXIT_MSG, ft_strlen(EXIT_MSG));
    if (cmd->args[1] && cmd->args[2])
    {
        write(2, EXIT_MANY_ARGS, ft_strlen(EXIT_MANY_ARGS));
        shell->status = 1;
        return;
    }
    exit_code = 0;
    if (cmd->args[1])
    {
        if (!valid_exit_code(cmd->args[1]))
        {
            write(2, EXIT_NUM_ERROR, ft_strlen(EXIT_NUM_ERROR));
            shell->exit_status = 255;
            exit(255);
        }
        exit_code = ft_atol(cmd->args[1]);
    }
    shell->exit_status = (unsigned char)(exit_code % 256);
    if (should_exit)
        exit(shell->exit_status);
}
