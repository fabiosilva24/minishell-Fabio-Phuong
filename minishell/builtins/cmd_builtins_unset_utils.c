/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:20:31 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:08:47 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	should_remove_var(char *env_var, char *args)
{
	return (!ft_strncmp(env_var, args, ft_strlen(args))
		&& env_var[ft_strlen(args)] == '=');
}

static char	**allocate_new_env(int size)
{
	char	**new_mass;

	new_mass = malloc(sizeof(char *) * size);
	if (!new_mass)
		return (NULL);
	return (new_mass);
}

void	free_new_mass_on_error(char **new_mass, int i)
{
	while (i > 0)
	{
		free(new_mass[i - 1]);
		i--;
	}
	free(new_mass);
}

int	copy_env_var(char **new_mass, char **envp, int i, int j)
{
	new_mass[i] = ft_strdup(envp[j]);
	if (!new_mass[i])
	{
		free_new_mass_on_error(new_mass, i);
		return (0);
	}
	return (1);
}

char	**remove_env_var(char *args, char **envp)
{
	int		i;
	int		j;
	char	**new_mass;

	i = 0;
	j = 0;
	new_mass = allocate_new_env(size_mass(envp));
	if (!new_mass)
		return (NULL);
	while (envp[j])
	{
		if (should_remove_var(envp[j], args))
		{
			j++;
			continue ;
		}
		if (!copy_env_var(new_mass, envp, i, j))
			return (NULL);
		i++;
		j++;
	}
	new_mass[i] = NULL;
	ft_free(envp);
	return (new_mass);
}
