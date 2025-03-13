/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:02:29 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 15:07:41 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_double_var(char *args, char **envp)
{
	int		i;
	char	*name_tmp;

	i = 0;
	name_tmp = malloc(sizeof(char) * (ft_sym_export(args) + 1));
	if (!name_tmp)
		return (NULL);
	while (args[i] != '\0' && args[i] != '=')
	{
		name_tmp[i] = args[i];
		i++;
	}
	name_tmp[i] = '\0';
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(name_tmp, envp[i],
				max(ft_strlen(name_tmp), ft_sym_export(envp[i]))))
			return (name_tmp);
		i++;
	}
	free(name_tmp);
	return (NULL);
}

char	**replace_env_var(char **envp, char *args, char *new)
{
	char	**new_env;
	int		j;

	j = 0;
	new_env = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_env)
		return (NULL);
	while (envp[j])
	{
		if (ft_strncmp(envp[j], new,
				max(ft_strlen(new), ft_sym_export(envp[j]))))
			new_env[j] = ft_strdup(envp[j]);
		else
			new_env[j] = ft_strdup(args);
		j++;
	}
	new_env[j] = NULL;
	return (new_env);
}

char	**add_env_var(char **envp, char *str, int free_old)
{
	char	**new_env;
	int		j;
	int		i;

	new_env = malloc(sizeof(char *) * (size_mass(envp) + 2));
	if (!new_env)
		return (NULL);
	j = 0;
	while (envp && envp[j])
	{
		new_env[j] = ft_strdup(envp[j]);
		if (!new_env[j])
		{
			while (j > 0)
			{
				free(new_env[j - 1]);
				j--;
			}
			free(new_env);
			return (NULL);
		}
		j++;
	}
	new_env[j] = ft_strdup(str);
	if (!new_env[j])
	{
		while (j > 0)
		{
			free(new_env[j - 1]);
			j--;
		}
		free(new_env);
		return (NULL);
	}
	j++;
	new_env[j] = NULL;
	if (free_old && envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
	return (new_env);
}

char	**extract_var_values(char **tmpmass)
{
	int		i;
	int		j;
	int		k;
	char	**after;

	after = malloc(sizeof(char *) * (size_mass(tmpmass) + 1));
	if (!after)
		return (NULL);
	i = -1;
	while (tmpmass[++i])
	{
		k = 0;
		after[i] = malloc(sizeof(char) * (ft_strlen(tmpmass[i])
					- ft_sym_export(tmpmass[i]) + 1));
		if (!after[i])
			return (NULL);
		j = ft_sym_export(tmpmass[i]);
		while (tmpmass[i][++j] != '\0')
			after[i][k++] = tmpmass[i][j];
		after[i][k] = '\0';
	}
	after[i] = NULL;
	return (after);
}
