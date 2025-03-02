
#include "../include/minishell.h"

void handle_cd_status(int j, int i, char *args, char *str, int *status)
{
    if (j == 1 && i)
        chdir(str);
    else if (j != 1)
        errmsg("minishell: cd: ", args, ": No such file or directory", -1, status);
    else if (j == 1 && !i)
        errmsg("minishell: cd: ", args, ": Not a directory", -1, status);
}

void check_directory_exists(char *path, int j, char *old_path, char *args, int *status)
{
    DIR             *dir;
    struct dirent   *entry;
    struct stat     status_file;
    int             if_is_cd_cmd;
    int             i;

    if_is_cd_cmd = j;
    entry = NULL;
    dir = opendir("./");
    entry = readdir(dir);
    j = 0;
    while (entry != NULL)
    {
        if (ft_strncmp(path, entry->d_name, ft_strlen(entry->d_name)) == 0)
        {
            stat(path, &status_file);
            j = 1;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    i = S_ISDIR(status_file.st_mode);
    handle_cd_status(j, i, args, path, status);
    if ((j != 1 || !i) || !if_is_cd_cmd)
        chdir(old_path);
}
void process_cd_path(char *args, char *old_path, int is_cd_builtin, int *status)
{
    char    **str;
    int     i;
    int     j;

    j = 0;
    i = ft_strlen(args);
    while (args[j] && args[j] != '/')
        j++;
    if (j != i)
        str = ft_split(args, '/');
    else
        str = ft_split(args, '\0');
    i = 0;
    while (str[i])
    {
        j = is_cd_builtin;
        check_directory_exists(str[i], j, old_path, args, status);
        i++;
    }
    ft_free(str);
}
