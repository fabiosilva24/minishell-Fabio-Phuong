#include "../include/minishell.h"

static char **copy_existing_env(char **envp, char **new_env)
{
    int i;

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
    new_env[i] = NULL;
    return (new_env);
}

char **add_to_envp(char **envp, char *str, int should_free)
{
    char **new_env;
    int i;

    if (!str || !envp)
        return (NULL);
    if (find_env_var(str, envp))
        return (envp);
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

static char *create_new_entry(char *key, char *new_value)
{
    char *tmp;
    char *new_entry;

    tmp = ft_strjoin(key, "=");
    if (!tmp)
        return (NULL);
    new_entry = ft_strjoin(tmp, new_value);
    free(tmp);
    return (new_entry);
}

int find_and_replace(char **envp, char *key, char *new_value, size_t key_len)
{
    int i;
    char *new_entry;

    i = 0;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], key, key_len) && envp[i][key_len] == '=')
        {
            new_entry = create_new_entry(key, new_value);
            if (!new_entry)
                return (0);
            free(envp[i]);
            envp[i] = new_entry;
            return (1);
        }
        i++;
    }
    return (0);
}

char **replace_env_var(char **envp, char *key, char *new_value)
{
    char *tmp;
    size_t key_len;
    char **result;

    if (!envp || !key || !new_value)
        return (envp);
    key_len = ft_sym_export(key);
    if (find_and_replace(envp, key, new_value, key_len))
        return (envp);
    entry = create_new_entry(key, new_value);
    if (!entry)
    {
        errmsg("minishell: export: memory allocation error", NULL, NULL, -1);
        return (envp);
    }
    result = add_to_envp(envp, entry, 1);
    free(entry);
    if (!result)
        return (envp);
    return (result);
}
