/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:49:59 by phoang            #+#    #+#             */
/*   Updated: 2025/03/15 17:56:25 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

enum e_token_type	token_determinator(char *token, int is_first)
{
	if (is_first)
		return (TOKEN_ARGUMENT);
	if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0
		|| strcmp(token, ">>") == 0 || strcmp(token, "<<") == 0)
		return (TOKEN_REDIRECT);
	if (strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	return (TOKEN_ARGUMENT);
}

t_token	*create_token(char *value, enum e_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenize_input(char *line)
{
	t_token	*head;
	int		is_first;
	char	*start;

	head = NULL;
	is_first = 1;
	start = line;
	head = process_line(line, start, &is_first);
	return (head);
}

void	*free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
	return (NULL);
}
