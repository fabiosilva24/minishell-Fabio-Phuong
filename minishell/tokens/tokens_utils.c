/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:36:37 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 17:30:30 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_token(t_token **head, t_token **current,
		int *in_double_quotes, int *in_single_quotes)
{
	*head = NULL;
	*current = NULL;
	*in_double_quotes = 0;
	*in_single_quotes = 0;
}

t_token	*process_line(char *line, char *start, int *is_first)
{
	t_token	*head;
	t_token	*current;
	int		in_double_quotes;
	int		in_single_quotes;

	init_token(&head, &current, &in_double_quotes, &in_single_quotes);
	while (*line)
	{
		handle_quotes_tk(line, &in_double_quotes, &in_single_quotes);
		if (ft_isspace(*line) && !in_double_quotes && !in_single_quotes)
		{
			current = handle_token(&line, start, &current, is_first);
			if (!head)
				head = current;
			start = line + 1;
		}
		line++;
	}
	if (start != line)
	{
		current = handle_token(&line, start, &current, is_first);
		if (!head)
			head = current;
	}
	return (head);
}

void	handle_quotes_tk(char *line, int *in_double_quotes,
			int *in_single_quotes)
{
	if (*line == '\"' && !(*in_single_quotes))
		*in_double_quotes = !(*in_double_quotes);
	else if (*line == '\'' && !(*in_double_quotes))
		*in_single_quotes = !(*in_single_quotes);
}

t_token	*create_and_link_token(char **line, char *start,
			t_token **current, int *is_first)
{
	char				*token;
	enum e_token_type	type;
	t_token				*new_token;

	**line = '\0';
	token = start;
	type = token_determinator(token, *is_first);
	new_token = create_token(token, type);
	if (!(*current))
		*current = new_token;
	else
		(*current)->next = new_token;
	*current = new_token;
	*is_first = 0;
	return (*current);
}

t_token	*handle_token(char **line, char *start,
			t_token **current, int *is_first)
{
	char				*token;
	enum e_token_type	type;
	t_token				*new_token;

	if (start != *line)
	{
		**line = '\0';
		token = start;
		type = token_determinator(token, *is_first);
		new_token = create_token(token, type);
		if (!(*current))
			*current = new_token;
		else
			(*current)->next = new_token;
		*current = new_token;
		*is_first = 0;
	}
	return (*current);
}
