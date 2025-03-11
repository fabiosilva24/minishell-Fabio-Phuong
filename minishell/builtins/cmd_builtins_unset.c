/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:20:31 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 15:29:08 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_env_var_present(char *args, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], args, ft_strlen(args))
			&& envp[i][ft_strlen(args)] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	**remove_env_var(char *args, char **envp)
{
	int		i;
	int		j;
	int		size;
	char	**new_mass;

	i = 0;
	j = 0;
	size = size_mass(envp);
	new_mass = malloc(sizeof(char *) * size);
	if (!new_mass)
		return (NULL);
	while (envp[j])
	{
		if (!ft_strncmp(envp[j], args, ft_strlen(args))
			&& envp[j][ft_strlen(args)] == '=')
		{
			free(envp[j]);
			j++;
			continue ;
		}
		new_mass[i++] = ft_strdup(envp[j++]);
	}
	new_mass[i] = NULL;
	free(envp);
	return (new_mass);
}

void	builtin_unset(char **args, char ***envp, int *status)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			errmsg("minishell: unset: `", args[i],
				"': not a valid identifier", -1, status);
		}
		else if (is_env_var_present(args[i], *envp))
		{
			*envp = remove_env_var(args[i], *envp);
		}
		i++;
	}
	*status = 0;
}
