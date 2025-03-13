/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/07 15:25:19 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/07 15:25:19 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	is_redirection(char *token)
{
	return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 || strcmp(token,
			"<") == 0 || strcmp(token, "<<") == 0);
}
static void	execute_piped_command(t_token *cmd_tokens, t_minishell *shell,
		int pipe_in, int pipe_out)
{
	pid_t pid;
	int token_count;
	char **arg;
	t_cmd cmd;
	t_token *head = cmd_tokens;
	int i;

	i = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}

	if (pid == 0) // Child process
	{
		// Setup input redirection from previous pipe
		if (pipe_in != STDIN_FILENO)
		{
			dup2(pipe_in, STDIN_FILENO);
			close(pipe_in);
		}
		// Setup output redirection to next pipe
		if (pipe_out != STDOUT_FILENO)
		{
			dup2(pipe_out, STDOUT_FILENO);
			close(pipe_out);
		}

		// Count tokens and convert to arguments
		token_count = count_tokens_until_pipe(cmd_tokens);
		arg = convert_tokens_to_argv_until_pipe(cmd_tokens, token_count);
		cmd.args = arg;

		while (i < token_count)
		{
			if (is_redirection(arg[i]))
			{
				apply_redirection(arg[i], arg[i + 1]);
				i++;
			}
			i++;
		}

		// Execute the command
		if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
			exec_extercmds(arg, shell, head);

		free_argv(arg);
		free_tokens(head);
		rl_clear_history();
		exit(shell->exit_status);
	}
}

// Count tokens up to a pipe or end of list
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

// Convert tokens to argv array, but only up to a pipe
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
	pid_t pids[1024];  // Store child PIDs
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

				execute_piped_command(cmd_start, shell, prev_pipe, STDOUT_FILENO);
				exit(shell->exit_status);
			}

			pid_count++;
			if (prev_pipe != STDIN_FILENO)
				close(prev_pipe);
			break;
		}
	}

	// Wait for all processes
	while (i < pid_count)
	{
		waitpid(pids[i++], &status, 0);
	}

	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);

	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

