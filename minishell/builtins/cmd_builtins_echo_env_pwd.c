
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
