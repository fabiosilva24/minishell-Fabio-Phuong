/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:59:43 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 18:18:38 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_child_process(t_token *current, t_minishell *shell,
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

void	handle_parent_process(t_pipe_data *pd, int *prev_pipe)
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

int	create_pipe(t_pipe_data *pd)
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

void	init_pipe_data(t_pipe_data *pd, t_token *tokens)
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

void	cleanup_pipes(t_pipe_data *pd)
{
	if (!pd->is_last)
	{
		close(pd->pipes[pd->cmd_count % 2][0]);
		close(pd->pipes[pd->cmd_count % 2][1]);
	}
}
