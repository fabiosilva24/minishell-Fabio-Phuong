/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:56:54 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 12:12:01 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_dollarsign(char *symbol, int *i, int len, t_minishell *shell)
{
	char var_name[256] = {0};
	int j;
	int env_index;
	char *env_value;

	env_value = 0;
	env_index = 0;
	j = 0;

	//(*i)++;
	if (*i >= len || !(isalnum(symbol[*i]) || symbol[*i] == '_'))
	{
		//(*i)++;
        printf("$"); // Print '$' if nothing valid comes after it
        return;
	}
	while (*i < len && (isalnum(symbol[*i]) || symbol[*i] == '_'))
	{
		if (j < 255)
			var_name[j++] = symbol[*i];
		(*i)++;
	}
	var_name[j] = '\0';
	env_index = 0;
	env_value = NULL;
	while (shell->environment[env_index])
	{
		if (strncmp(shell->environment[env_index], var_name, strlen(var_name)) == 0)
		{
			env_value = strchr(shell->environment[env_index], '=') + 1;
			break ;
		}
		env_index++;
	}

	if (env_value)
		printf("%s", env_value);
	else
		printf("%s", env_value);
	(*i)--;
}

/*void double_quotes(char *symbol)
{
	int len;
	int i;
	char *aditional_input;
	
	len = strlen(symbol);
	i = 1;
	if (symbol[0] == '\"')
	{
		while (i < len && symbol[i] != '\"')
		{
			if (symbol[i] == '$')
				handle_dollarsign(symbol, &i, len);
			else
				printf("%c", symbol[i]);
			i++;
		}
		if (i == len)
		{
			printf("\n> ");
			aditional_input = readline(NULL);
			if (aditional_input)
			{
				//double_quotes(aditional_input);
				free(aditional_input);
			}
		}
	}
}*/

void double_quotes(char *symbol, t_minishell *shell)
{
	int len;
	int i;
	char *input;

	len = strlen(symbol);
	i = 1;
	if (symbol[0] == '\"')
	{
		while (i < len && symbol[i] != '\"')
		{

			if (symbol[i] == '$')
			{
				i++;
				handle_dollarsign(symbol, &i, len, shell);
			}
			else
				printf("%c", symbol[i]);
			i++;
		}
		while (i == len)
		{
			input = readline("> ");
			signal(SIGINT, handle_sigint);
    		signal(SIGQUIT, SIG_IGN);
			if (!input)
				break;
			free(input);
		}
	}
}

void single_quotes(char *symbol)
{
    int len;
    int i;

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
		{
		    signal(SIGINT, handle_sigint);
    		signal(SIGQUIT, SIG_IGN);
			printf("> Error unclosed quotes");
		}
	}
}