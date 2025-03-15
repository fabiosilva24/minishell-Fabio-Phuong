/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:36:37 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 16:48:04 by fsilva-p         ###   ########.fr       */
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

static void	cleanup_and_exit(char **arg, t_token *head,
		t_minishell *shell, int exit_status)
{
	if (arg)
		free_argv(arg);
	if (head)
		free_tokens(head);
	if (shell)
	{
		rl_clear_history();
		cleanup_shell(shell);
	}
	exit(exit_status);
}

static void	cleanup_pipe_fds(int pipe_in, int pipe_out)
{
	if (pipe_in != STDIN_FILENO)
		close(pipe_in);
	if (pipe_out != STDOUT_FILENO)
		close(pipe_out);
}

void	execute_child_process1(t_token *cmd_tokens, t_minishell *shell,
		int pipe_in, int pipe_out)
{
	char	**arg;
	t_cmd	cmd;
	t_token	*head;
	int		token_count;
	int		exit_status;

	head = cmd_tokens;
	token_count = count_tokens_until_pipe(cmd_tokens);
	setup_pipe_io(pipe_in, pipe_out);
	arg = convert_tokens_to_argv_until_pipe(cmd_tokens, token_count);
	cmd.args = arg;
	handle_redirectionss(arg, token_count);
	exit_status = shell->exit_status;
	if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
		exec_extercmds(arg, shell, head);
	cleanup_pipe_fds(pipe_in, pipe_out);
	cleanup_and_exit(arg, head, shell, exit_status);
}

t_token	*find_next_pipe(t_token *current)
{
	t_token	*pipe_token;

	pipe_token = current;
	while (pipe_token && pipe_token->type != TOKEN_PIPE)
		pipe_token = pipe_token->next;
	return (pipe_token);
}

t_token	*get_next_command(t_token *current)
{
	t_token	*pipe_token;

	pipe_token = find_next_pipe(current);
	if (pipe_token)
		return (pipe_token->next);
	return (NULL);
}
