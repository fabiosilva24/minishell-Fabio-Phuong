/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_dollarsign.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:05:39 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 18:39:41 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	extract_variable_name(const char *symbol, int *i,
	int len, char *var_name)
{
	int	j;

	j = 0;
	while (*i < len && (isalnum(symbol[*i])
			|| symbol[*i] == '_'))
	{
		if (j < 255)
			var_name[j++] = symbol[*i];
		(*i)++;
	}
	var_name[j] = '\0';
}

char	*find_env_value(const char *var_name, char **environment)
{
	int		env_index;
	size_t	var_name_len;

	var_name_len = strlen(var_name);
	env_index = 0;
	while (environment[env_index])
	{
		if (strncmp(environment[env_index], var_name, var_name_len) == 0
			&& environment[env_index][var_name_len] == '=')
		{
			return (strchr(environment[env_index], '=') + 1);
		}
		env_index++;
	}
	return (NULL);
}
