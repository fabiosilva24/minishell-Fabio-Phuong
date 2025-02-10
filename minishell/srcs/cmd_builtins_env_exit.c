#include "../minishell.h"

/*
#define EXIT_NUM_ERROR "exit: numeric argument required\n"
#define EXIT_MANY_ARGS "exit: too many arguments\n"
#define EXIT_MSG "exit\n"
#define ENV_ERROR "minishell: env: error retrieving environment variables\n"
#define MAX_PATH_LEN 4096
#define PWD_ERROR "minishell: pwd: error retrieving current directory\n"
void	builtin_env(void);
static int is_numeric_arg(const char *arg);
static void exit_error(void);
void	builtin_exit(t_cmd *cmd, int should_exit);
*/

void    builtin_env(void)
{
    int     env_index;
    ssize_t write_ret;

    env_index = 0;
    g_line.status = 0;
    while (g_line.envp[env_index])
    {
        if (ft_strchr(g_line.envp[env_index], '='))
        {
            write_ret = write(1, g_line.envp[env_index], ft_strlen(g_line.envp[env_index]));
            if (write_ret == -1 || write(1, "\n", 1) == -1)
            {
                write(2, ENV_ERROR, ft_strlen(ENV_ERROR));
                g_line.status = 1;
                return ;
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

static void exit_error(void)
{
    write(2, EXIT_NUM_ERROR, ft_strlen(EXIT_NUM_ERROR));
    exit(255);
}

void    builtin_exit(t_cmd *cmd, int should_exit)
{
    long    exit_code;

    if (!cmd)
    {
        g_line.status = 1;
        return ;
    }
    if (should_exit && !(cmd->args[1] && cmd->args[2]))
        write(1, EXIT_MSG, ft_strlen(EXIT_MSG));
    if (cmd->args[1] && cmd->args[2])
    {
        write(2, EXIT_MANY_ARGS, ft_strlen(EXIT_MANY_ARGS));
        g_line.status = 1;
        return ;
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
