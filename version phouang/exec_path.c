#include "../minishell.h"

static char    *try_direct_path(char *cmd)
{
	char	*dup;

	if (!ft_strchr(cmd, '/'))
		return (NULL);
	if (is_executable(cmd))
	{
		dup = ft_strdup(cmd);
		if (!dup)
    {
        print_cmd_error("malloc", ": memory allocation failed\n");
        return (NULL);
    }
		return (dup);
	}
	return (NULL);
}

char	**get_path_dirs(char **envp)
{
	int		i;
	char	*path_value;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			return (ft_split(path_value, ':'));
		}
		i++;
	}
	return (NULL);
}

static int	is_executable(const char *path)
{
	return (access(path, F_OK) == 0 && access(path, X_OK) == 0);
}

char *find_cmd_path(char **path_dirs, char *cmd)
{
    char    *full_path;
    char    *temp;
    int     i;

    if (!cmd || !path_dirs)
        return (NULL);
    full_path = try_direct_path(cmd);
    if (full_path)
        return (full_path);
    i = 0;
    while (path_dirs[i])
    {
        temp = ft_strjoin(path_dirs[i], "/");
        if (!temp)
            return (NULL);
        full_path = ft_strjoin(temp, cmd);
        free(temp);
        if (!full_path)
				{
            print_cmd_error("malloc", ": memory allocation failed\n");
            return (NULL);
        }
        if (is_executable(full_path))
            return (full_path);
        free(full_path);
        i++;
    }
    return (NULL);
}

char	*get_exec_path(t_cmd *cmd, char **envp)
{
	t_path	path;
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		print_cmd_error("(null)", ": command not found\n");
		return (NULL);
	}
	cmd_path = try_direct_path(cmd->args[0]);
	if (cmd_path)
		return (cmd_path);
	path.dirs = get_path_dirs(envp);
	if (!path.dirs)
	{
		print_cmd_error(cmd->args[0], ": command not found\n");
		return (NULL);
	}
	cmd_path = find_cmd_path(path.dirs, cmd->args[0]);
	free_tab(path.dirs);
	if (!cmd_path)
	{
		print_cmd_error(cmd->args[0], ": command not found\n");
		return (NULL);
	}
	return (cmd_path);
}
