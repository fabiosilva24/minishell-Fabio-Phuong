/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42luxembourg.>  +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/14 15:52:49 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 16:03:30 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(char *token)
{
	return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 || strcmp(token,
			"<") == 0 || strcmp(token, "<<") == 0);
}
int	count_tokens_until_pipe(t_token *tokens)
{
	int count;
	t_token *current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}
	return (count);
}
char	**convert_tokens_to_argv_until_pipe(t_token *tokens, int token_count)
{
	char **argv;
	int i;
	t_token *current;

	i = 0;
	if (!tokens || token_count <= 0)
		return (NULL);
	current = tokens;
	argv = malloc(sizeof(char *) * (token_count + 1));
	if (!argv)
		return (NULL);
	while (current && i < token_count && current->type != TOKEN_PIPE)
	{
		argv[i] = ft_strdup(current->value);
		i++;
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}
void	process_pipes(t_token *tokens, t_minishell *shell)
{
	int pipes[2][2];
	pid_t pid;
	t_token *current;
	int is_first;
	int is_last;
	int prev_pipe;
	int cmd_count;

	cmd_count = 0;
	prev_pipe = -1;
	is_last = 0;
	is_first = 1;
	current = tokens;

	t_token *tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE || !tmp->next)
			cmd_count++;
		tmp = tmp->next;
	}
	while (current)
	{
		// Find next pipe token
		t_token *pipe_token = current;
		while (pipe_token && pipe_token->type != TOKEN_PIPE)
			pipe_token = pipe_token->next;

		is_last = (pipe_token == NULL);
		if (!is_last)
		{
			if (pipe(pipes[cmd_count % 2]) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}

		if (pid == 0)
		{
			signal(SIGPIPE, SIG_DFL);
			if (!is_first && prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (!is_last)
			{
				dup2(pipes[cmd_count % 2][1], STDOUT_FILENO);
				close(pipes[cmd_count % 2][0]);
				close(pipes[cmd_count % 2][1]);
			}

			execute_child_process1(current, shell, STDIN_FILENO,
				STDOUT_FILENO);
			exit(shell->exit_status);
		}
		if (!is_first && prev_pipe != -1)
			close(prev_pipe);

		if (!is_last)
		{
			close(pipes[cmd_count % 2][1]);
			prev_pipe = pipes[cmd_count % 2][0];
		}
		is_first = 0;
		cmd_count++;
		// Move to next command
		current = pipe_token ? pipe_token->next : NULL;
	}

	// Wait for all child processes
	int status;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}
