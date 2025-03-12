/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:30:03 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:10:55 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_execution_error(t_cmd *cmd, t_minishell *shell)
{
	t_error	err;

	err.s1 = "minishell: ";
	err.s2 = cmd->args[0];
	err.s3 = ": No such file or directory";
	err.code = -1;
	err.status = &(shell->status);
	errmsg(&err);
	exit(127);
}

static void	execute_child_process(t_cmd *cmd, char **envp, t_minishell *shell)
{
	char	**paths;
	char	*name;

	paths = extract_path_directories(envp);
	if (!paths)
	{
		handle_execution_error(cmd, shell);
		return ;
	}
	name = find_executable(paths, cmd->args, &(shell->status));
	ft_free(paths);
	if (!name)
		exit(127);
	if (execve(name, cmd->args, envp) == -1)
	{
		handle_execution_error(cmd, shell);
	}
	free(name);
}

void	execute(t_cmd *cmd, char ***envp, t_minishell *shell)
{
	pid_t	p;

	if (exec_builtins(cmd, envp, shell))
		return ;
	p = fork();
	if (!p)
		execute_child_process(cmd, *envp, shell);
	else
		waitpid(p, &(shell->status), 0);
}
