void builtin_pwd(int *status)
{
    char cwd[MAX_PATH_LEN];

    if (!getcwd(cwd, sizeof(cwd)))
    {
        write(2, PWD_ERROR, ft_strlen(PWD_ERROR));
        *status = 1;
        return;
    }
    if (write(1, cwd, strlen(cwd)) == -1 ||
        write(1, "\n", 1) == -1)
    {
        *status = 1;
        return;
    }
    *status = 0;
}

static int has_n_flag(const char *arg)
{
    int i;

    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return (0);
    i = 1;
    while (arg[++i])
    {
        if (arg[i] != 'n')
            return (0);
    }
    return (1);
}

static void print_echo_args(char **args, int start_idx, int *status)
{
    int i;

    i = start_idx;
    while (args[i])
    {
        if (write(1, args[i], ft_strlen(args[i])) == -1)
        {
            *status = 1;
            return;
        }
        if (args[i + 1])
        {
            if (write(1, " ", 1) == -1)
            {
                *status = 1;
                return;
            }
        }
        i++;
    }
}

void builtin_echo(t_cmd *cmd, int *status)
{
    int i;
    int newline;

    if (!cmd)
    {
        *status = 1;
        return;
    }
    i = 1;
    newline = 1;
    *status = 0;
    if (!cmd->args[1])
    {
        write(1, "\n", 1);
        return;
    }
    while (cmd->args[i] && has_n_flag(cmd->args[i]))
    {
        newline = 0;
        i++;
    }
    print_echo_args(cmd->args, i, status);
    if (newline && *status == 0)
    {
        if (write(1, "\n", 1) == -1)
            *status = 1;
    }
}
