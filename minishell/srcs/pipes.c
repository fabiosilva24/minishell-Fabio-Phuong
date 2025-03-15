/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: fsilva-p <fsilva-p@student.42luxembourg.>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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
	int		count;
	t_token	*current;

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
	char	**argv;
	int		i;
	t_token	*current;

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

static void	handle_child_process(t_token *current, t_minishell *shell,
		int prev_pipe, t_pipe_data *pd)
{
	signal(SIGPIPE, SIG_DFL);
	if (!pd->is_first && prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (!pd->is_last)
	{
		dup2(pd->pipes[pd->cmd_count % 2][1], STDOUT_FILENO);
		close(pd->pipes[pd->cmd_count % 2][0]);
		close(pd->pipes[pd->cmd_count % 2][1]);
	}
	execute_child_process1(current, shell, STDIN_FILENO, STDOUT_FILENO);
	exit(shell->exit_status);
}

static void	handle_parent_process(t_pipe_data *pd, int *prev_pipe)
{
	if (!pd->is_first && *prev_pipe != -1)
		close(*prev_pipe);
	if (!pd->is_last)
	{
		close(pd->pipes[pd->cmd_count % 2][1]);
		*prev_pipe = pd->pipes[pd->cmd_count % 2][0];
	}
	pd->is_first = 0;
	pd->cmd_count++;
}

static int	create_pipe(t_pipe_data *pd)
{
	if (!pd->is_last)
	{
		if (pipe(pd->pipes[pd->cmd_count % 2]) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

static void	init_pipe_data(t_pipe_data *pd, t_token *tokens)
{
	t_token	*tmp;

	pd->cmd_count = 0;
	pd->is_first = 1;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE || !tmp->next)
			pd->cmd_count++;
		tmp = tmp->next;
	}
	pd->cmd_count = 0;
}

static void	cleanup_pipes(t_pipe_data *pd)
{
	if (!pd->is_last)
	{
		close(pd->pipes[pd->cmd_count % 2][0]);
		close(pd->pipes[pd->cmd_count % 2][1]);
	}
}

static void	wait_for_children(t_minishell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}

static int	handle_pipe_creation(t_pipe_data *pd, t_token *current)
{
	pd->is_last = !find_next_pipe(current);
	if (!create_pipe(pd))
	{
		cleanup_pipes(pd);
		return (0);
	}
	return (1);
}

static int	handle_fork_process(t_pipe_data *pd, t_token *current,
		t_minishell *shell, int *prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		cleanup_pipes(pd);
		return (0);
	}
	if (pid == 0)
		handle_child_process(current, shell, *prev_pipe, pd);
	handle_parent_process(pd, prev_pipe);
	return (1);
}

void	process_pipes(t_token *tokens, t_minishell *shell)
{
	t_pipe_data	pd;
	int			prev_pipe;
	t_token		*current;

	init_pipe_data(&pd, tokens);
	prev_pipe = -1;
	current = tokens;
	while (current)
	{
		if (!handle_pipe_creation(&pd, current)
			|| !handle_fork_process(&pd, current, shell, &prev_pipe))
			return ;
		current = get_next_command(current);
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	wait_for_children(shell);
}
