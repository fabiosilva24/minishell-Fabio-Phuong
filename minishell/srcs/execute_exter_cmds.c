/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_exter_cmds.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/07 15:25:26 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/07 15:25:26 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	filter_redirections(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (argv[i])
	{
		if ((strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0
				|| strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0
				|| strcmp(argv[i], "|") == 0) && argv[i + 1])
		{
			i += 2;
			continue ;
		}
		argv[j++] = argv[i++];
	}
	argv[j] = NULL;
}

static char	*find_executable2(char *cmd, char *path_env)
{
	static char	full_path[1024];
	char		path_env_copy[MAX_PATH_LEN];
	char		*path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (cmd);
		else
			return (NULL);
	}
	ft_strncpy(path_env_copy, path_env, sizeof(path_env_copy));
	path = ft_strtok(path_env_copy, ":");
	while (path != NULL)
	{
		ft_strncpy(full_path, path, sizeof(full_path));
		ft_strncat(full_path, "/", sizeof(full_path) - ft_strlen(full_path) - 1);
		ft_strncat(full_path, cmd, sizeof(full_path) - ft_strlen(full_path) - 1);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		path = ft_strtok(NULL, ":");
	}
	return (NULL);
}

static void	execute_child_process2(char **argv, t_minishell *shell,
		t_token *tokens, char *path_env)
{
	char	*exec_path;

	exec_path = find_executable2(argv[0], path_env);
	if (exec_path != NULL)
	{
		execve(exec_path, argv, shell->environment);
		perror("Error with execve");
	}
	else
	{
		fprintf(stderr, "miaoushell: command not found: %s\n", argv[0]);
	}
	free_argv(argv);
	free_tokens(tokens);
	exit(127);
}

static void	execute_forked_process(char **argv, t_minishell *shell,
		t_token *tokens, char *path_env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execute_child_process2(argv, shell, tokens, path_env);
	}
	else if (pid < 0)
	{
		perror("Error with fork");
		shell->exit_status = 127;
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else
			shell->exit_status = 127;
	}
}

void	exec_extercmds(char **argv, t_minishell *shell, t_token *tokens)
{
	char	*path_env;

	if (!argv || !argv[0])
		return ;
	filter_redirections(argv);
	if (!argv[0])
		return ;
	path_env = getenv("PATH");
	if (!path_env)
	{
		perror("Error: PATH Environment Variable not found");
		return ;
	}
	execute_forked_process(argv, shell, tokens, path_env);
}
