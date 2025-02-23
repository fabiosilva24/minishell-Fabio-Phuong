char *find_env_var(char *args, char **envp)
{
    int index;
    char *var_name;
    char *value;
    size_t var_len;

    if (!args || !envp)
        return (NULL);
    if (!ft_strchr(args, '='))
        return (NULL);
    var_len = ft_sym_export(args);
    var_name = ft_strndup(args, var_len);
    if (!var_name)
        return (NULL);
    index = 0;
    value = NULL;
    while (envp[index])
    {
        if (!ft_strncmp(var_name, envp[index], var_len) &&
            envp[index][var_len] == '=')
        {
            value = ft_strdup(envp[index] + var_len + 1);
            break;
        }
        index++;
    }
    free(var_name);
    return (value);
}

static void swap_strings(char **str1, char **str2)
{
    char *tmp;

    tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}

static void bubble_sort_iteration(char **envp, int size, int i)
{
    int k;

    k = 0;
    while (k < size - i - 1)
    {
        if (ft_strcmp(envp[k], envp[k + 1]) > 0)
            swap_strings(&envp[k], &envp[k + 1]);
        k++;
    }
}

char **sort_env_vars(char **envp)
{
    int i;
    int size;

    if (!envp)
        return (NULL);
    size = size_mass(envp);
    i = 0;
    while (i < size - 1)
    {
        bubble_sort_iteration(envp, size, i);
        i++;
    }
    return (envp);
}

char **remove_quotes_from_env(char **envp)
{
	int		i;
	int		pos;
	char	**new_env;

	if (!envp)
		return (NULL);
	new_env = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		pos = ft_sym_export(envp[i]);
    new_env[i] = ft_strndup(envp[i] + pos, ft_strlen(envp[i]) - pos);
        if (!new_env[i])
        {
            free_tab(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[i] = NULL;
    return (new_env);
}
