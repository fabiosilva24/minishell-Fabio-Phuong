#include "../minishell.h"

void builtin_env(char **envp, int *status)
{
    int     env_index;
    ssize_t write_ret;

    env_index = 0;
    *status = 0;
    while (envp[env_index])
    {
        if (ft_strchr(envp[env_index], '='))
        {
            write_ret = write(1, envp[env_index], ft_strlen(envp[env_index]));
            if (write_ret == -1 || write(1, "\n", 1) == -1)
            {
                write(2, ENV_ERROR, ft_strlen(ENV_ERROR));
                *status = 1;
                return;
            }
        }
        env_index++;
    }
}

static int is_numeric_arg(const char *arg)
{
    int     char_index;
    size_t  len;

    if (!arg || !arg[0])
        return (0);
    char_index = (arg[0] == '-' || arg[0] == '+');
    len = ft_strlen(arg);
    if (len == 1 && char_index == 1)
        return (0);
    while (arg[char_index] && ft_isdigit(arg[char_index]))
        char_index++;
    return ((size_t)char_index == len);
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
        if (!is_numeric_arg(cmd->args[1]))
        {
            write(2, EXIT_NUM_ERROR, ft_strlen(EXIT_NUM_ERROR));
            exit(255);
        }
        exit_code = ft_atol(cmd->args[1]);
    }
    exit((unsigned char)exit_code);
}

/*
static void exit_error(void)
{
    write(2, EXIT_NUM_ERROR, ft_strlen(EXIT_NUM_ERROR));
    exit(255);
}
*/
