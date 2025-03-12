/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builtins_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:59:37 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 16:05:12 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit)
{
	if (!cmd)
	{
		shell->status = 1;
		return ;
	}
	if (cmd->args[1] && cmd->args[2])
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		return ;
	}
	if (cmd->args[1])
		handle_exit_with_arg(cmd, shell, should_exit);
	else
	{
		shell->exit_status = 0;
		if (should_exit)
		{
			printf("exit\n");
			exit(0);
		}
	}
}
