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
			exec_extercmds(arg, shell);

		free(arg);
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
	int prev_pipe;
	t_token *cmd_start = tokens;
	t_token *current = tokens;
	int status;
	pid_t last_pid;

	int original_stdin;
	int original_stdout;
	int token_count;
	char **arg;
	t_cmd cmd;
	int i;

	i = 0;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	prev_pipe = STDIN_FILENO;

	while (current)
	{
		// Find the next pipe token
		t_token *pipe_token = cmd_start;
		while (pipe_token && pipe_token->type != TOKEN_PIPE)
			pipe_token = pipe_token->next;

		// If we found a pipe, set up piping
		if (pipe_token)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return ;
			}

			execute_piped_command(cmd_start, shell, prev_pipe, pipe_fd[1]);
			close(pipe_fd[1]);

			if (prev_pipe != STDIN_FILENO)
				close(prev_pipe);

			prev_pipe = pipe_fd[0];
			current = pipe_token;
			cmd_start = (current->next) ? current->next : NULL;
			current = current->next;
		}
		else
		{
			// Last command in the pipeline
			last_pid = fork();
			if (last_pid == 0)
			{
				if (prev_pipe != STDIN_FILENO)
				{
					dup2(prev_pipe, STDIN_FILENO);
					close(prev_pipe);
				}

				token_count = count_tokens_until_pipe(cmd_start);
				arg = convert_tokens_to_argv_until_pipe(cmd_start,
						token_count);
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
				if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
					exec_extercmds(arg, shell);

				free(arg);
				exit(shell->exit_status);
			}

			if (prev_pipe != STDIN_FILENO)
				close(prev_pipe);

			waitpid(last_pid, &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);

			break ; // Exit the loop as we've processed all commands
		}
	}

	// Restore original stdin and stdout
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}
