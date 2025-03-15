/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:52:49 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 19:52:52 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	wait_for_children(t_minishell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}

int	handle_pipe_creation(t_pipe_data *pd, t_token *current)
{
	pd->is_last = !find_next_pipe(current);
	if (!create_pipe(pd))
	{
		cleanup_pipes(pd);
		return (0);
	}
	return (1);
}

int	handle_fork_process(t_pipe_data *pd, t_token *current,
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
