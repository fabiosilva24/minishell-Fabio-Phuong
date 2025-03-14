/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:37:15 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 19:44:32 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_after_char(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' || s[i] == c)
	{
		if (s[i] == c)
			return ((char *)s + (i + 1));
		i++;
	}
	return (NULL);
}

static void	handle_home_directory(void)
{
	chdir(getenv("HOME"));
}

static void	handle_home_path(char *args, char *old_path,
	int if_cdcmd, int *status)
{
	char	*path;

	chdir(getenv("HOME"));
	path = ft_strdup(get_after_char(args, '/'));
	process_cd_path(path, old_path, if_cdcmd, status);
	free(path);
}

static void	handle_regular_path(char *args, char *old_path,
	int if_cdcmd, int *status)
{
	*status = 0;
	process_cd_path(args, old_path, if_cdcmd, status);
}

char	**change_directory(char **args, int if_cdcmd, char **envp, int *status)
{
	char	*old_path;
	char	cwd[MAX_PATH_LEN + 1];

	old_path = ft_strdup(getcwd(cwd, sizeof(cwd)));
	if (!old_path)
		return (envp);
	if (if_cdcmd && (!args[1] || !ft_strncmp(args[1], "~", ft_strlen(args[1]))))
		handle_home_directory();
	else if (args[1])
	{
		if (ft_strncmp(args[1], "~/", 2) == 0)
			handle_home_path(args[1], old_path, if_cdcmd, status);
		else
			handle_regular_path(args[1], old_path, if_cdcmd, status);
	}
	else if (if_cdcmd && !ft_strncmp(args[1], "..", ft_strlen("..")))
		chdir("..");
	if (if_cdcmd)
		envp = update_pwd_oldpwd(envp, old_path, status);
	else
		free(old_path);
	return (envp);
}
