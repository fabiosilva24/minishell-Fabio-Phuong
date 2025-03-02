
#include "../include/minishell.h"

char **update_pwd_oldpwd(char **envp, char *old_path, int *status)
{
    char *pwd;
    char cwd[MAX_PATH_LEN];
    char *new_old_path;
    (void)status;

    new_old_path = ft_strjoin("OLDPWD=", old_path);
    free(old_path);
    pwd = ft_strjoin("PWD=", getcwd(cwd, 4096));
    envp = replace_env_var(envp, pwd, "PWD=");
    if (is_env_var_present("OLDPWD=", envp))
        envp = replace_env_var(envp, new_old_path, "OLDPWD=");
    else
        envp = add_env_var(envp, new_old_path, 1);
    free(new_old_path);
    free(pwd);
    return (envp);
}

char *get_after_char(const char *s, int c)
{
    int i = 0;
    while (s[i] != '\0' || s[i] == c)
    {
        if (s[i] == c)
            return ((char *)s + (i + 1));
        i++;
    }
    return (NULL);
}

char **change_directory(char **args, int if_is_cd_cmd, char **envp, int *status)
{
    char *path;
    char *old_path;
    char cwd[MAX_PATH_LEN];

    old_path = ft_strdup(getcwd(cwd, 4097));
    if (if_is_cd_cmd && (!args[1] || !ft_strncmp(args[1], "~", ft_strlen(args[1]))))
        chdir(getenv("HOME"));
    else if (args[1])
    {
        if (ft_strncmp(args[1], "~/", 2) == 0)
        {
            chdir(getenv("HOME"));
            path = ft_strdup(get_after_char(args[1], '/'));
            process_cd_path(path, old_path, if_is_cd_cmd, status);
            free(path);
        }
        else
						*status = 0;
            process_cd_path(args[1], old_path, if_is_cd_cmd, status);
    }
    else if (if_is_cd_cmd && !ft_strncmp(args[1], "..", ft_strlen("..")))
        chdir("..");
    if (if_is_cd_cmd)
        envp = update_pwd_oldpwd(envp, old_path, status);
    return (envp);
}
