/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/01/30 17:56:54 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 12:12:01 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_inside_quotes(char *symbol, int *i, int len,
		t_minishell *shell)
{
	while (*i < len && symbol[*i] != '\"')
	{
		if (symbol[*i] == '$')
		{
			(*i)++;
			handle_dollarsign(symbol, i, len, shell);
		}
		else
		{
			printf("%c", symbol[*i]);
		}
		(*i)++;
	}
}

static void	handle_unclousedquote(int len)
{
	char	*input;
	int		i;

	i = len;
	while (i == len)
	{
		input = readline("> ");
		signal(SIGINT, handle_sigint);
		if (!input)
			break ;
		free(input);
	}
}

void	handle_dollarsign(char *symbol, int *i, int len, t_minishell *shell)
{
	char	var_name[256];
	char	*env_value;

	memset(var_name, 0, sizeof(var_name));
	if (*i >= len || !(isalnum(symbol[*i]) || symbol[*i] == '_'))
	{
		printf("$");
		return ;
	}
	extract_variable_name(symbol, i, len, var_name);
	env_value = find_env_value(var_name, shell->environment);
	if (env_value)
		printf("%s", env_value);
	else
		printf("invalid variable name");
	(*i)--;
}

void	double_quotes(char *symbol, t_minishell *shell)
{
	int	len;
	int	i;
	int	quote_closed;

	len = strlen(symbol);
	i = 1;
	quote_closed = 0;
	if (symbol[0] == '\"')
	{
		process_inside_quotes(symbol, &i, len, shell);
		if (i < len && symbol[i] == '\"')
		{
			i++;
			quote_closed = 1;
		}
		if (quote_closed && i < len)
		{
			printf("%s", &symbol[i]);
		}
		else if (!quote_closed)
		{
			handle_unclousedquote(len);
		}
	}
}

void	single_quotes(char *symbol)
{
	int	len;
	int	i;

	len = strlen(symbol);
	i = 1;
	if (symbol[0] == '\'')
	{
		while (i < len && symbol[i] != '\'')
		{
			printf("%c", symbol[i]);
			i++;
		}
		if (i == len)
			handle_unclousedquote(len);
	}
}
