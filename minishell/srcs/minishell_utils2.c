/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:14:22 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 19:14:34 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cleanup_argv(char **argv, int i)
{
	while (--i >= 0)
	{
		free(argv[i]);
		argv[i] = NULL;
	}
	free(argv);
}

char	**allocate_argv(int token_count)
{
	char	**argv;

	argv = malloc(sizeof(char *) * (token_count + 1));
	if (!argv)
		return (NULL);
	return (argv);
}

char	**convert_tokens_to_argv(t_token *tokens, int token_count)
{
	char	**argv;
	int		i;

	if (!tokens || token_count <= 0)
		return (NULL);
	argv = allocate_argv(token_count);
	if (!argv)
		return (NULL);
	i = 0;
	while (tokens && i < token_count)
	{
		argv[i] = ft_strdup(tokens->value);
		if (!argv[i])
		{
			cleanup_argv(argv, i);
			return (NULL);
		}
		tokens = tokens->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	handle_redirection_error(t_token *current)
{
	if (!current->next)
	{
		perror("Syntax error: missing file for redirection");
		return ;
	}
}

int	setup_redirection(t_token *current)
{
	char	*redir_symbol;
	char	*filename;

	redir_symbol = current->value;
	filename = current->next->value;
	return (apply_redirection(redir_symbol, filename));
}
