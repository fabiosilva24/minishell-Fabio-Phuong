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
	int count = 0;
	t_token *current = tokens;

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
	int i = 0;
	t_token *current;

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
	int pipe_fd[2];
	int prev_pipe = STDIN_FILENO;
	t_token *cmd_start = tokens;
	t_token *current = tokens;
	pid_t pids[1024]; // Store child PIDs
	int pid_count = 0;
	int status;
	int i;
	int original_stdin;
	int original_stdout;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	i = 0;

	while (current)
	{
		// Find the next pipe token
		t_token *pipe_token = cmd_start;
		while (pipe_token && pipe_token->type != TOKEN_PIPE)
			pipe_token = pipe_token->next;

		// If there's another command after this
		if (pipe_token)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return ;
			}

			pids[pid_count] = fork();
			if (pids[pid_count] == 0) // Child
			{
				if (prev_pipe != STDIN_FILENO)
				{
					dup2(prev_pipe, STDIN_FILENO);
					close(prev_pipe);
				}
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);

				execute_piped_command(cmd_start, shell, prev_pipe, pipe_fd[1]);
				exit(shell->exit_status);
			}

			pid_count++;
			close(pipe_fd[1]);
			if (prev_pipe != STDIN_FILENO)
				close(prev_pipe);
			prev_pipe = pipe_fd[0];

			cmd_start = pipe_token->next;
			current = cmd_start;
		}
		else
		{
			// Last command
			pids[pid_count] = fork();
			if (pids[pid_count] == 0)
			{
				if (prev_pipe != STDIN_FILENO)
				{
					dup2(prev_pipe, STDIN_FILENO);
					close(prev_pipe);
				}

				execute_piped_command(cmd_start, shell, prev_pipe,
					STDOUT_FILENO);
				free_tokens(tokens);
				exit(shell->exit_status);
			}

			pid_count++;
			if (prev_pipe != STDIN_FILENO)
				close(prev_pipe);
			break ;
		}
	}

	// Wait for all processes
	while (i < pid_count)
	{
		waitpid(pids[i++], &status, 0);
		free_tokens(tokens);
	}

	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);

	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}
