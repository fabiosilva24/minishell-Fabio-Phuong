/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:45:49 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:03:04 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_cd_status(t_cd *cd)
{
	t_error	err;

	if (cd->j != 0)
	{
		if (cd->i == 0)
		{
			err.s1 = "minishell: cd: ";
			err.s2 = cd->args;
			err.s3 = ": No such file or directory";
			err.code = -1;
			err.status = cd->status;
			errmsg(&err);
		}
		else
		{
			err.s1 = "minishell: cd: ";
			err.s2 = cd->str;
			err.s3 = ": No such file or directory";
			err.code = -1;
			err.status = cd->status;
			errmsg(&err);
		}
	}
}

void	check_directory_exists(t_cd *cd)
{
	if (!search_directory(cd))
		cd->i = 0;
	if (!cd->i)
		handle_cd_status(cd);
}

int	search_directory(t_cd *cd)
{
	DIR				*dir;
	struct dirent	*entry;
	struct stat		status_file;

	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, cd->path, ft_strlen(cd->path) + 1))
		{
			stat(entry->d_name, &status_file);
			cd->i = S_ISDIR(status_file.st_mode);
			closedir(dir);
			return (1);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (0);
}

void	process_cd_path(char *path, char *old_path,
	int is_cd_builtin, int *status)
{
	t_cd	cd;

	cd.path = path;
	cd.old_path = old_path;
	cd.args = path;
	cd.str = NULL;
	cd.status = status;
	cd.j = chdir(path);
	cd.i = 1;
	if (cd.j == -1)
		check_directory_exists(&cd);
	if (!cd.j && is_cd_builtin)
		*status = 0;
}

char	*create_oldpwd_var(char *old_path)
{
	char	*new_old_path;

	new_old_path = ft_strjoin("OLDPWD=", old_path);
	if (!new_old_path)
	{
		free(old_path);
		return (NULL);
	}
	return (new_old_path);
}
