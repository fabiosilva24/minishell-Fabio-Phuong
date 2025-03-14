/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:11:06 by phoang            #+#    #+#             */
/*   Updated: 2025/03/14 21:19:52 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_basic_builtins(t_cmd *cmd, char **envp, t_minishell *shell)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		builtin_echo(cmd, &(shell->status), shell);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		builtin_pwd(&(shell->status));
	else if (!ft_strcmp(cmd->args[0], "env"))
		builtin_env(envp, &(shell->status));
	else
		return (0);
	return (1);
}

static int	handle_env_builtins(t_cmd *cmd, char ***envp, t_minishell *shell)
{
	if (!ft_strcmp(cmd->args[0], "export"))
		*envp = builtin_export(cmd->args, *envp, &(shell->status));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		builtin_unset(cmd->args, envp, &(shell->status));
	else if (!ft_strcmp(cmd->args[0], "cd"))
	{
		*envp = change_directory(cmd->args, 1, *envp, &(shell->status));
		shell->exit_status = shell->status;
		return (1);
	}
	else
		return (0);
	return (1);
}

int	exec_builtins(t_cmd *cmd, char ***envp, t_minishell *shell)
{
	if (!ft_strcmp(cmd->args[0], "exit"))
	{
		builtin_exit(cmd, shell, 1);
		return (1);
	}
	if (handle_basic_builtins(cmd, *envp, shell)
		|| handle_env_builtins(cmd, envp, shell))
	{
		shell->exit_status = shell->status;
		return (1);
	}
	return (0);
}

char	**extract_path_directories(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}
