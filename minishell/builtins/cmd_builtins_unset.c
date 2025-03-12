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
	int		i;
	t_error	err;

	i = 1;
	err.code = -1;
	err.status = status;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			err.s1 = "minishell: unset: `";
			err.s2 = args[i];
			err.s3 = "': not a valid identifier";
			errmsg(&err);
		}
		else if (is_env_var_present(args[i], *envp))
		{
			*envp = remove_env_var(args[i], *envp);
		}
		i++;
	}
	*status = 0;
}
