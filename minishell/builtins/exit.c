/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phoang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:38:47 by phoang            #+#    #+#             */
/*   Updated: 2025/03/11 15:53:08 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_exit_variable_value(char **envp, char *line,
	int len, int last_exit_status)
{
	char	*var_name;
	int		i;
	int		eq_pos;

	var_name = malloc(sizeof(char) * len);
	if (!var_name)
		return (0);
	ft_strlcpy(var_name, line, len);
	i = -1;
	while (envp[++i])
	{
		eq_pos = ft_len_eq(envp[i]);
		if (eq_pos == (int)ft_strlen(envp[i])
			|| !ft_strncmp(var_name, envp[i],
				max(eq_pos, ft_strlen(var_name))))
		{
			free(var_name);
			if (eq_pos == (int)ft_strlen(envp[i]))
				return (ft_strdup(""));
			return (ft_strdup(envp[i] + eq_pos + 1));
		}
	}
	if (!ft_strncmp("?", var_name, 1))
	{
		free(var_name);
		return (ft_itoa(last_exit_status));
	}
	free(var_name);
	return (ft_strdup(""));
}

void	replace_exit_status(char **line, int start,
	int len, char *value, int *i)
{
	char	*new_line;
	int		value_len;
	char	*old_line;

	value_len = 0;
	if (value)
		value_len = ft_strlen(value);
	old_line = *line;
	new_line = malloc(sizeof(char) * (ft_strlen(old_line) + value_len));
	if (!new_line)
		return ;
	ft_strlcpy(new_line, old_line, start + 1);
	if (value)
		ft_strlcpy(new_line + start, value, ft_strlen(value) + 1);
	ft_strlcpy(new_line + start + value_len, old_line + start + len,
		ft_strlen(old_line + start + len) + 1);
	free(old_line);
	*line = new_line;
	(*i) = start;
	if (value)
		free(value);
}

void	extract_and_replace_exit_status(int *i, char **line, char **envp,
	int last_exit_status)
{
	int		count;
	char	*value;

	count = 0;
	(*i)++;
	if (ft_isdigit((*line)[*i]) || (*line)[*i] == '?')
	{
		count = 1;
		(*i)++;
	}
	else
	{
		while ((*line)[*i] != '\0' && ((*line)[*i] == '_'
				|| ft_isdigit((*line)[*i])
				|| ft_isalpha((*line)[*i])))
		{
			(*i)++;
			count++;
		}
	}
	value = get_exit_variable_value(envp, *line + (*i - count), count + 1,
			last_exit_status);
	replace_exit_status(line, (*i) - count - 1, count + 1, value, i);
}

void	expand_exit_status(char **line, char **envp, int last_exit_status)
{
	int	i;
	int	single_quote_count;
	int	double_quote_count;

	i = 0;
	single_quote_count = 0;
	double_quote_count = 0;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == '"' && single_quote_count % 2 == 0)
			double_quote_count++;
		if ((*line)[i] == '\'')
			single_quote_count++;
		if ((single_quote_count % 2 == 0 || double_quote_count % 2 == 1)
			&& (*line)[i] == '$')
			extract_and_replace_exit_status(&i, line, envp, last_exit_status);
		else
			i++;
	}
}
