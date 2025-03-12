/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_export_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:08:07 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 15:20:01 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	validate_export_name(const char *arg, int *status)
{
	t_error	err;

	if (!ft_isalpha(arg[0]) || arg[0] == '=')
	{
		err.s1 = "minishell: export: `";
		err.s2 = arg;
		err.s3 = "': not a valid identifier";
		err.code = -1;
		err.status = status;
		errmsg(&err);
		return (0);
	}
	return (1);
}

static char	**process_export_args(char **args, char **envp, int *status)
{
	int		i;
	char	*new_value;

	i = 1;
	while (args[i])
	{
		if (!validate_export_name(args[i], status))
			return (envp);
		new_value = find_double_var(args[i], envp);
		envp = change_envp(new_value, args, i, envp);
		i++;
	}
	*status = 0;
	return (envp);
}

char	**builtin_export(char **args, char **envp, int *status)
{
	if (!args[1])
	{
		print_sorted_env(envp, status);
		return (envp);
	}
	return (process_export_args(args, envp, status));
}
