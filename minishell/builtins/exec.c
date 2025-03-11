/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:30:03 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 15:38:33 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_builtins(t_cmd *cmd, char ***envp, t_minishell *shell)
{
	if (!ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])))
	{
		builtin_exit(cmd, shell, 1);
		return (1);
	}
	else if (!ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])))
		builtin_echo(cmd, &(shell->status), shell);
	else if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])))
		builtin_pwd(&(shell->status));
	else if (!ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])))
		builtin_env(*envp, &(shell->status));
	else if (!ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])))
		*envp = builtin_export(cmd->args, *envp, &(shell->status));
	else if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		builtin_unset(cmd->args, envp, &(shell->status));
	else if (!ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])))
	{
		*envp = change_directory(cmd->args, 1, *envp, &(shell->status));
		shell->exit_status = shell->status;
		return (1);
	}
	else
		return (0);
	shell->exit_status = shell->status;
	return (1);
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

char	*find_executable(char **paths, char **cmd_flags, int *status)
{
	char	*full_cmd;
	char	*cmd;
	int		i;

	i = 0;
	if (!access(cmd_flags[0], F_OK))
		return (ft_strdup(cmd_flags[0]));
	while (paths && paths[i])
	{
		cmd = ft_strjoin(paths[i], "/");
		full_cmd = ft_strjoin(cmd, cmd_flags[0]);
		free(cmd);
		if (!access(full_cmd, F_OK))
			return (full_cmd);
		free(full_cmd);
		i++;
	}
	errmsg("minishell: ", cmd_flags[0], ": command not found",
		-127, status);
	return (NULL);
}

void	execute(t_cmd *cmd, char ***envp, t_minishell *shell)
{
	char	**paths;
	char	*name;
	pid_t	p;

	if (exec_builtins(cmd, envp, shell))
		return ;
	p = fork();
	if (!p)
	{
		paths = extract_path_directories(*envp);
		if (!paths)
			errmsg("minishell: ", cmd->args[0],
				": command not found", -127, &(shell->status));
		name = find_executable(paths, cmd->args, &(shell->status));
		ft_free(paths);
		if (execve(name, cmd->args, *envp) == -1)
		{
			errmsg("minishell: ", cmd->args[0],
				": command not found", -127, &(shell->status));
			exit(shell->exit_status);
		}
		free(name);
	}
	else
		waitpid(p, &(shell->status), 0);
}
