
#include "../include/minishell.h"

int is_env_var_present(char *args, char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], args, ft_strlen(args)) == 0)
            return (1);
        i++;
    }
    return (0);
}

char **remove_env_var(char *args, char **envp)
{
    int     i;
    int     j;
    char    **new_mass;

    new_mass = malloc(sizeof(char *) * size_mass(envp));
    if (!new_mass)
        return (NULL);
    i = 0;
    j = 0;
    while (envp[j])
    {
        if (ft_strncmp(envp[j], args, ft_strlen(args)) == 0)
            j++;
        if (!envp[j])
            break;
        new_mass[i] = ft_strdup(envp[j]);
        i++;
        j++;
    }
    new_mass[i] = NULL;
    ft_free(envp);
    return (new_mass);
}

char **builtin_unset(char **args, char **envp, int *status)
{
    int i;
    int j;
    int k;

    i = 0;
    while (args[++i])
    {
        j = 0;
        k = 0;
        while (args[i][j])
        {
            if (args[i][j] == '=')
            {
                errmsg("minishell: unset: `", args[i],
                    "': not a valid identifier", -1, status);
                k = 1;
                break;
            }
            j++;
        }
        if (k == 0 && is_env_var_present(args[i], envp))
            envp = remove_env_var(args[i], envp);
    }
    *status = 0;
    return (envp);
}
