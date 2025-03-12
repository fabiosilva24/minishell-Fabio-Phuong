/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:15:45 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:21:31 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_command_error(char **cmd_flags, int *status)
{
	t_error	err;

	err.s1 = "minishell: ";
	err.s2 = cmd_flags[0];
	err.s3 = ": command not found";
	err.code = -127;
	err.status = status;
	errmsg(&err);
}

static char	*try_direct_access(char **cmd_flags)
{
	if (!access(cmd_flags[0], F_OK))
		return (ft_strdup(cmd_flags[0]));
	return (NULL);
}

static char	*try_path_combinations(char **paths, char *cmd)
{
	char	*full_cmd;
	char	*path_with_slash;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		path_with_slash = ft_strjoin(paths[i], "/");
		full_cmd = ft_strjoin(path_with_slash, cmd);
		free(path_with_slash);
		if (!access(full_cmd, F_OK))
			return (full_cmd);
		free(full_cmd);
		i++;
	}
	return (NULL);
}

char	*find_executable(char **paths, char **cmd_flags, int *status)
{
	char	*direct_path;
	char	*path_result;

	direct_path = try_direct_access(cmd_flags);
	if (direct_path)
		return (direct_path);
	path_result = try_path_combinations(paths, cmd_flags[0]);
	if (path_result)
		return (path_result);
	print_command_error(cmd_flags, status);
	return (NULL);
}
