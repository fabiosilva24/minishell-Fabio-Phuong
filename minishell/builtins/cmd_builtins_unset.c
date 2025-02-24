#include "../include/minishell.h"

static int copy_or_skip_var(char **new_envp, char **envp, char *var_name, int *i)
{
    size_t var_len;
    char *current_var;

    current_var = envp[*i];
    var_len = ft_strlen(var_name);
    if (ft_strncmp(current_var, var_name, var_len) == 0 &&
        (current_var[var_len] == '=' || current_var[var_len] == '\0'))
    {
        return (1);
    }
    new_envp[*i] = ft_strdup(current_var);
    if (!new_envp[*i])
    {
        free_tab(new_envp);
        return (-1);
    }
    return (0);
}

char	**remove_variable_from_env(char *var_name, char **envp)
{
	int		i;
	int		j;
	char	**new_envp;
	int		result;

	if (!var_name || !envp)
		return (envp);
	new_envp = malloc(sizeof(char *) * size_mass(envp));
	if (!new_envp)
		return (envp);
	i = 0;
	j = 0;
	while (envp[i])
	{
		result = copy_or_skip_var(new_envp, envp, var_name, &i);
		if (result == -1)
		{
			free_tab(new_envp);
			return (envp);
		}
		if (result == 0)
			j++;
		i++;
	}
	new_envp[j] = NULL;
	free_tab(envp);
	return (new_envp);
}

int is_variable_set(char *var_name, char **envp)
{
    int     i;
    int     var_len;
    char    *current_var;

    if (!var_name || !envp)
        return (0);

    var_len = ft_strlen(var_name);
    i = 0;
    while (envp[i])
    {
        current_var = envp[i];
        if (ft_strncmp(current_var, var_name, var_len) == 0 &&
            (current_var[var_len] == '=' || current_var[var_len] == '\0'))
            return (1);
        i++;
    }
    return (0);
}

char **builtin_unset(char **args, char **envp, int *status)
{
    int     i;
    char    **new_envp;

    if (!args || !envp || !status)
        return (envp);
    if (!args[1])
        return (envp);
    i = 0;
    while (args[++i])
    {
        if (!is_valid_identifier(args[i]))
        {
            errmsg("minishell: unset: `", args[i], "': not a valid identifier\n", -1);
            *status = 1;
            continue;
        }
        if (is_variable_set(args[i], envp))
        {
            new_envp = remove_variable_from_env(args[i], envp);
		if (new_envp)
		{
			free_tab(envp);
			envp = new_envp;
		}
	}
    }
	return (envp);
}
