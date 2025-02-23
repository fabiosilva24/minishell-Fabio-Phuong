#include "../minishell.h"

void validate_and_cd(char *path, char *args)
{
    DIR *dir;

    if (!path)
        return;
    dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        if (access(path, X_OK) == 0)
        {
            if (chdir(path) == -1)
                errmsg("minishell: cd: ", args, ": Permission denied", -1);
        }
        else
            errmsg("minishell: cd: ", args, ": Permission denied", -1);
    }
    else
        errmsg("minishell: cd: ", args, ": No such file or directory", -1);
}

void builtin_cd(char **args, char ***envp)
{
    char *old_pwd;
    char *new_pwd;

    if (!args || !args[0] || !envp || !*envp)
        return;
    if (args[1] && args[2])
        return (errmsg("minishell: cd: too many arguments", NULL, NULL, -1));
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return (errmsg("minishell: cd: error retrieving current directory", NULL, NULL, -1));
    if (!args[1] || *args[1] == '\0' || !ft_strcmp(args[1], "~"))
        handle_relative_path(args, *envp);
    else
        validate_and_cd(args[1], args[1]);
    new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
				if (ft_strcmp(old_pwd, new_pwd) != 0)
        	update_pwd_variables(envp, old_pwd);
        free(new_pwd);
    }
    free(old_pwd);
}
