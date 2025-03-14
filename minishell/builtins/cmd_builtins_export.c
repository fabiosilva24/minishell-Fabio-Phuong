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

int	copy_or_replace_var(char **new_env, char **envp, char *args,
	t_replace_data *data)
{
	if (ft_strncmp(envp[data->j], data->new,
			max(ft_strlen(data->new), ft_sym_export(envp[data->j]))))
		new_env[data->j] = ft_strdup(envp[data->j]);
	else
		new_env[data->j] = ft_strdup(args);
	if (!new_env[data->j])
		return (0);
	return (1);
}

char	**replace_env_var(char **envp, char *args, char *new)
{
	char			**new_env;
	t_replace_data	data;

	data.j = 0;
	data.new = new;
	new_env = malloc(sizeof(char *) * (size_mass(envp) + 1));
	if (!new_env)
		return (NULL);
	while (envp[data.j])
	{
		if (!copy_or_replace_var(new_env, envp, args, &data))
		{
			free_new_mass_on_error(new_env, data.j);
			return (NULL);
		}
		data.j++;
	}
	new_env[data.j] = NULL;
	ft_free(envp);
	return (new_env);
}

int	copy_existing_vars(char **new_env, char **envp, int *j)
{
	while (envp && envp[*j])
	{
		new_env[*j] = ft_strdup(envp[*j]);
		if (!new_env[*j])
		{
			free_new_mass_on_error(new_env, *j);
			return (0);
		}
		(*j)++;
	}
	return (1);
}

int	add_new_var(char **new_env, char *str, int j)
{
	new_env[j] = ft_strdup(str);
	if (!new_env[j])
	{
		free_new_mass_on_error(new_env, j);
		return (0);
	}
	return (1);
}
