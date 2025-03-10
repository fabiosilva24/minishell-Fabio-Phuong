/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:57:01 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 12:26:16 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_only_that_char(char *str, char c)
{
	while (*str)
	{
		if (*str != c)
			return (0);
		str++;
	}
	return (1);
}

void	execute_echo(t_token *tokens)
{
	int		arg_count;
	t_token	*current;
	char	**args;
	int		i;

	i = 1;
	arg_count = 0;
	current = tokens->next;
	args = malloc(sizeof(char *) * (arg_count + 1));
	args[arg_count++] = "echo";
	while (current)
	{
		arg_count++;
		current = current->next;
	}
	args[arg_count] = NULL;
	while (i < arg_count)
	{
		parse_input(args[1]);
		if (i < arg_count - 1)
			printf(" ");
		i++;
	}
	printf("\n");
	free(args);
}

void	my_echo(int argc, char **argv)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while (argc > 1 && argv[i][0] == '-'
		&& is_only_that_char(argv[i] + 1, 'n'))
	{
		newline = 0;
		i++;
	}
	while (i < argc)
	{
		parse_input(argv[i]);
		if (i < argc - 1)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
