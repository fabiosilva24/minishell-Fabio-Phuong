/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:45:49 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 14:45:54 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_cd_status(t_cd *cd)
{
	t_error	err;

	if (cd->j == 0)
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
	DIR				*dir;
	struct dirent	*entry;
	struct stat		status_file;
	int				found;

	dir = opendir(".");
	if (!dir)
		return ;
	found = 0;
	while ((entry = readdir(dir)))
	{
		if (!ft_strncmp(entry->d_name, cd->path, ft_strlen(cd->path) + 1))
		{
			found = 1;
			stat(entry->d_name, &status_file);
			cd->i = S_ISDIR(status_file.st_mode);
			break ;
		}
	}
	closedir(dir);
	if (!found)
		cd->i = 0;
	if (!cd->i)
		handle_cd_status(cd);
}

void	process_cd_path(char *path, char *old_path, int is_cd_builtin, int *status)
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

char	**update_pwd_oldpwd(char **envp, char *old_path, int *status)
{
	char	*pwd;
	char	cwd[MAX_PATH_LEN];
	char	*new_old_path;

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
