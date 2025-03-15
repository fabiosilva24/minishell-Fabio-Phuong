/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:12:08 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 18:20:35 by fsilva-p         ###   ########.fr       */
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
