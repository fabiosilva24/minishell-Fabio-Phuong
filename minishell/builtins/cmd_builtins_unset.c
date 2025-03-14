/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 21:18:43 by phoang            #+#    #+#             */
/*   Updated: 2025/03/14 21:18:48 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
