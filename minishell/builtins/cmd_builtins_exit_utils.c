/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_exit_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:04:26 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:04:55 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_basic_validity(const char *arg)
{
	if (!arg || !arg[0] || ((arg[0] == '-' || arg[0] == '+') && !arg[1]))
		return (0);
	return (1);
}

static int	check_digits_and_length(const char *arg, int *len)
{
	int	i;

	i = (arg[0] == '-' || arg[0] == '+');
	*len = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		(*len)++;
		i++;
	}
	return (1);
}

static int	check_max_long_value(const char *arg, int len)
{
	const char	*max_long;

	if (len > 19)
		return (0);
	if (len == 19)
	{
		max_long = "9223372036854775807";
		if (arg[0] == '-')
			max_long = "9223372036854775808";
		if (ft_strncmp(arg + (arg[0] == '-'
					|| arg[0] == '+'), max_long, 19) > 0)
			return (0);
	}
	return (1);
}

static int	valid_exit_code(const char *arg)
{
	int	len;

	if (!check_basic_validity(arg))
		return (0);
	if (!check_digits_and_length(arg, &len))
		return (0);
	if (!check_max_long_value(arg, len))
		return (0);
	return (1);
}

void	handle_exit_with_arg(t_cmd *cmd, t_minishell *shell,
		int should_exit)
{
	long	exit_code;

	if (!valid_exit_code(cmd->args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		shell->exit_status = 2;
		return ;
	}
	exit_code = ft_atol(cmd->args[1]);
	shell->exit_status = (unsigned char)exit_code;
	if (should_exit)
	{
		printf("exit\n");
		exit(shell->exit_status);
	}
}
