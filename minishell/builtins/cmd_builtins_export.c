#include "../include/minishell.h"

char **extract_variable_names(char **envp)
{
    int     i;
    char    **variable_names;

    if (!envp)
        return (NULL);
    variable_names = malloc(sizeof(char *) * (size_mass(envp) + 1));
    if (!variable_names)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        if (ft_strchr(envp[i], '='))
            variable_names[i] = get_variable_name(envp[i]);
        else
            variable_names[i] = ft_strdup(envp[i]);
        if (!variable_names[i])
        {
            free_variable_names(variable_names, i);
            return (NULL);
        }
        i++;
    }
    variable_names[i] = NULL;
    return (variable_names);
}

char **prepare_sorted_env(char **envp)
{
    char **tmpmass;

    if (!envp)
        return (NULL);
    tmpmass = new_envp(envp);
    if (!tmpmass)
    {
        errmsg("minishell: export: memory allocation error", NULL, NULL, -1);
        return (NULL);
    }
    return (sort_env_vars(tmpmass));
}

int prepare_export_data(char **tmpmass, char ***before, char ***after)
{
    *before = extract_variable_names(tmpmass);
    *after = remove_quotes_from_env(tmpmass);
    if (!*before || !*after)
    {
        if (*before)
            free_tab(*before);
        if (*after)
            free_tab(*after);
        return (0);
    }
    return (1);
}

void    print_export_without_args(char **envp)
{
    int     i;
    char    **tmpmass;
    char    **before;
    char    **after;

    tmpmass = prepare_sorted_env(envp);
    if (!tmpmass)
        return;
    if (!prepare_export_data(tmpmass, &before, &after))
    {
        free_tab(tmpmass);
        return;
    }
    i = -1;
    while (tmpmass[++i])
        print_env_entry(before[i], after[i], tmpmass[i]);
    free_tab(tmpmass);
    free_tab(before);
    free_tab(after);
}

char **builtin_export(char **args, char **envp)
{
    int     i;
    char    *new_value;

    if (!args || !envp)
        return (envp);
    if (!args[1])
    {
        print_export_without_args(envp);
        return (envp);
    }
    i = 0;
    while (args[++i])
    {
        if (!is_valid_identifier(args[i]))
        {
            errmsg("minishell: export: `", args[i], "': not a valid identifier", -1);
            continue;
        }
	if (ft_strchr(args[i], '='))
	{
            new_value = find_env_var(args[i], envp);
            update_envp(new_value, args[i], &envp);
            free(new_value);
        }
        else
            update_envp(NULL, args[i], &envp);
    }
    return (envp);
}
