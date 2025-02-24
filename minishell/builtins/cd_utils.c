#include "../include/minishell.h"

char *get_cd_env_value(char **envp, char *key)
{
    char *var;
    char *value;

    if (!key || !envp)
        return (NULL);
    var = find_env_var(key, envp);
    if (!var)
        return (NULL);
    value = ft_strchr(var, '=');
    if (!value || !*(value + 1))
    {
        free(var);
        return (NULL);
    }
    value = ft_strdup(value + 1);
    free(var);
    return (value);
}

void update_pwd_variables(char ***envp, char *old_pwd)
{
    char *current_pwd;
    char *new_pwd;
    char *new_oldpwd;

    current_pwd = getcwd(NULL, 0);
    if (!current_pwd)
        return;
    new_pwd = ft_strjoin("PWD=", current_pwd);
    new_oldpwd = ft_strjoin("OLDPWD=", old_pwd);
    free(current_pwd);
    if (!new_pwd || !new_oldpwd)
    {
        free(new_pwd);
        free(new_oldpwd);
        return;
    }
    *envp = replace_env_var(*envp, "PWD", new_pwd);
    free(new_pwd);
    if (!*envp)
    {
        free(new_oldpwd);
        return;
    }
    *envp = replace_env_var(*envp, "OLDPWD", new_oldpwd);
    free(new_oldpwd);
}

void handle_cd_error(char *path)
{
    if (path)
        errmsg("minishell: cd: ", path, ": Permission denied", -1);
    else
        errmsg("minishell: cd: ", "error", NULL, -1);
}

void handle_absolute_path(char *path)
{
   if (ft_strlen(path) >= MAX_PATH_LEN)
	 {
        errmsg("minishell: cd: ", path, ": Path too long", -1);
        return;
    }
   if (chdir(path) == -1)
       handle_cd_error(path);
}

static char *resolve_home_path(char *arg, char **envp)
{
    char *home;
    char *path;

    home = get_cd_env_value(envp, "HOME");
    if (!home)
        return (errmsg("minishell: cd: ", NULL, "HOME not set", -1), NULL);
    if (!arg || *arg == '\0' || !strcmp(arg, "~"))
        return (home);
    if (!strncmp(arg, "~/", 2))
    {
        if (ft_strlen(home) + ft_strlen(arg) - 1 >= MAX_PATH_LEN)
        {
            free(home);
            return (errmsg("minishell: cd: ", NULL, "Path too long", -1), NULL);
        }
        path = ft_strjoin(home, arg + 1);
        free(home);
        if (!path)
            return (errmsg("minishell: cd: ", NULL, "Memory allocation failed", -1), NULL);
        return (path);
    }
    free(home);
    return (NULL);
}

void handle_relative_path(char **args, char **envp)
{
    char *path;

    if (!args[1] || !strcmp(args[1], "~") || !strncmp(args[1], "~/", 2))
        path = resolve_home_path(args[1], envp);
    else if (!strcmp(args[1], "-"))
    {
        path = get_cd_env_value(envp, "OLDPWD");
        if (!path)
        {
            errmsg("minishell: cd: ", NULL, "OLDPWD not set", -1);
            return;
        }
    }
    else
    {
        handle_absolute_path(args[1]);
        return;
    }
    if (!path)
        return;
    if (ft_strlen(path) >= MAX_PATH_LEN)
        errmsg("minishell: cd: ", NULL, "Path too long", -1);
    else if (chdir(path) == -1)
        handle_cd_error(path);
    free(path);
}
