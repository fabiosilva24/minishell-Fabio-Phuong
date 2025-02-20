#include "../minishell.h"

char **add_to_envp(char **envp, char *str, int should_free)
{
    char **new_env;
    int i;

    if (!str || !envp)
        return (NULL);
    new_env = malloc(sizeof(char *) * (size_mass(envp) + 2));
    if (!new_env)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        new_env[i] = ft_strdup(envp[i]);
        if (!new_env[i])
        {
            free_tab(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[i] = ft_strdup(str);
    if (!new_env[i])
    {
        free_tab(new_env);
        return (NULL);
    }
    new_env[i + 1] = NULL;
    if (should_free)
        free_tab(envp);
    return (new_env);
}

char *find_env_var(char *args, char **envp)
{
    int     index;
    char    *var_name;
    size_t  var_len;

    if (!args || !envp)
        return (NULL);
    var_len = ft_sym_export(args);
    var_name = ft_strndup(args, var_len);
    if (!var_name)
        return (NULL);
    index = 0;
    while (envp[index])
    {
        if (!ft_strncmp(var_name, envp[index], var_len) &&
            envp[index][var_len] == '=')
        {
            free(var_name);
            return (ft_strdup(envp[index]));
        }
        index++;
    }
    free(var_name);
    return (NULL);
}

char    **replace_env_var(char **envp, char *key, char *new_value)
{
    int     i;
    char    *new_entry;
    char    *tmp;
    size_t  key_len;

    if (!envp || !key || !new_value)
        return (envp);
    key_len = ft_sym_export(key);
    i = 0;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], key, key_len) && envp[i][key_len] == '=')
        {
            tmp = ft_strjoin(key, "=");
            if (!tmp)
                return (envp);
            new_entry = ft_strjoin(tmp, new_value);
            free(tmp);
            if (!new_entry)
                return (envp);
            free(envp[i]);
            envp[i] = new_entry;
            return (envp);
        }
        i++;
    }
    tmp = ft_strjoin(key, "=");
    if (!tmp)
        return (envp);
    envp = add_to_envp(envp, tmp, 1);
    free(tmp);
    return (envp);
}

char **sort_env_vars(char **envp)
{
    int     i;
    int     k;
    int     size;
    char    *tmp;

    if (!envp)
        return (NULL);
    size = size_mass(envp);
    i = 0;
    while (i < size - 1)
    {
        k = 0;
        while (k < size - i - 1)
        {
            if (ft_strcmp(envp[k], envp[k + 1]) > 0)
            {
                tmp = envp[k];
                envp[k] = envp[k + 1];
                envp[k + 1] = tmp;
            }
            k++;
        }
        i++;
    }
    return (envp);
}

char **remove_quotes_from_env(char **envp)
{
	int		i;
	int		j;
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
    if (pos < 0 || pos >= (int)ft_strlen(envp[i]))
            pos = 0;
        j = ft_strlen(envp[i]) - pos;
        new_env[i] = ft_strndup(envp[i] + pos, j);
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
