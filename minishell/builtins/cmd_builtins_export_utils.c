#include "../include/minishell.h"

char **extract_var_names(char **env_copy_tmp)
{
    int     i;
    int     j;
    char    **var_names;

    if (!env_copy_tmp)
        return (NULL);
    var_names = malloc(sizeof(char *) * (size_mass(env_copy_tmp) + 1));
    if (!var_names)
        return (NULL);
    i = -1;
    while (env_copy_tmp[++i])
    {
        j = -1;
        var_names[i] = malloc(sizeof(char) * (ft_sym_export(env_copy_tmp[i]) + 1));
        if (!var_names[i])
            return (NULL);
        if (!ft_strchr(env_copy_tmp[i], '='))
            var_names[i] = ft_strdup(env_copy_tmp[i]);
        else
        {
            while (env_copy_tmp[i][++j] != '=')
                var_names[i][j] = env_copy_tmp[i][j];
            var_names[i][j] = '\0';
        }
    }
    var_names[i] = NULL;
    return (var_names);
}

void print_sorted_env(char **envp)
{
    int     i;
    char    **env_copy_tmp;
    char    **var_names;
    char    **var_values;

    i = 0;
    if (!env_copy_tmp)
        return;
    env_copy_tmp = new_envp(envp);
    env_copy_tmp = sort_env_vars(env_copy_tmp);
    var_names = extract_var_names(env_copy_tmp);
    var_values = extract_var_values(env_copy_tmp);
    while (env_copy_tmp[i])
    {
        if (!ft_strchr(env_copy_tmp[i], '='))
            printf("declare -x %s\n", var_names[i]);
        else
            printf("declare -x %s=\"%s\"\n", var_names[i], var_values[i]);
        i++;
    }
    ft_free(env_copy_tmp);
    ft_free(var_names);
    ft_free(var_values);
}

char **change_envp(char *new_value, char **args, int i, char **envp)
{
    if (!envp)
        return (NULL);
    if (new_value && ft_strrchr(args[i], '='))
        envp = replace_env_var(envp, args[i], new_value);
    else if (!new_value)
        envp = add_env_var(envp, args[i], 0);
    return envp;
}

char **builtin_export(char **args, char **envp, int *status)
{
    int     i;
    int     j;
    char    *new_value;

    if (!envp)
        return (NULL);
    if (!args[1])
        print_sorted_env(envp);
    i = 0;
    while (args[++i])
    {
        j = -1;
        new_value = find_double_var(args[i], envp);
        while (args[i][++j])
        {
            if (!ft_isalpha(args[i][0]) || args[i][0] == '=')
                errmsg("minishell: export: `", args[i],
                    "': not a valid identifier", -1, status);
            else
                envp = change_envp(new_value, args, i, envp);
            break;
        }
        if (new_value)
            free(new_value);
    }
    return (envp);
}
