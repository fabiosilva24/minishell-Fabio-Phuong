/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:36:37 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 20:57:50 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_pipe_io(int pipe_in, int pipe_out)
{
	if (pipe_in != STDIN_FILENO)
	{
		dup2(pipe_in, STDIN_FILENO);
		close(pipe_in);
	}
	if (pipe_out != STDOUT_FILENO)
	{
		dup2(pipe_out, STDOUT_FILENO);
		close(pipe_out);
	}
}

void	handle_redirectionss(char **arg, int token_count)
{
	int	i;

	i = 0;
	while (i < token_count)
	{
		if (is_redirection(arg[i]))
		{
			apply_redirection(arg[i], arg[i + 1]);
			i++;
		}
		i++;
	}
}

void	execute_child_process1(t_token *cmd_tokens, t_minishell *shell,
		int pipe_in, int pipe_out)
{
	char	**arg;
	t_cmd	cmd;
	t_token	*head;
	int		token_count;

	head = cmd_tokens;
	token_count = count_tokens_until_pipe(cmd_tokens);
	setup_pipe_io(pipe_in, pipe_out);
	arg = convert_tokens_to_argv_until_pipe(cmd_tokens, token_count);
	cmd.args = arg;
	handle_redirectionss(arg, token_count);
	if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
		exec_extercmds(arg, shell, head);
	free_argv(arg);
	free_tokens(head);
	rl_clear_history();
	exit(shell->exit_status);
}

void	execute_piped_command(t_token *cmd_tokens, t_minishell *shell,
		int pipe_in, int pipe_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		execute_child_process1(cmd_tokens, shell, pipe_in, pipe_out);
}
