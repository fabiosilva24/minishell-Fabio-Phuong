/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:38:47 by phoang            #+#    #+#             */
/*   Updated: 2025/03/12 20:24:10 by phoang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	replace_exit_status(char **line, t_replace *rep)
{
	char	*new_line;
	int		value_len;
	char	*old_line;

	value_len = 0;
	if (rep->value)
		value_len = ft_strlen(rep->value);
	old_line = *line;
	new_line = malloc(sizeof(char) * (ft_strlen(old_line) + value_len));
	if (!new_line)
		return ;
	ft_strlcpy(new_line, old_line, rep->start + 1);
	if (rep->value)
		ft_strlcpy(new_line + rep->start, rep->value,
			ft_strlen(rep->value) + 1);
	ft_strlcpy(new_line + rep->start + value_len,
		old_line + rep->start + rep->len,
		ft_strlen(old_line + rep->start + rep->len) + 1);
	*line = new_line;
	*(rep->i) = rep->start;
	if (rep->value)
		free(rep->value);
}

static int	count_variable_length(char *line, int *i)
{
	int	count;

	count = 0;
	(*i)++;
	if (ft_isdigit(line[*i]) || line[*i] == '?')
	{
		count = 1;
		(*i)++;
	}
	else
	{
		while (line[*i] != '\0' && (line[*i] == '_'
				|| ft_isdigit(line[*i])
				|| ft_isalpha(line[*i])))
		{
			(*i)++;
			count++;
		}
	}
	return (count);
}

void	extract_and_replace_exit_status(int *i, char **line, char **envp,
	int last_exit_status)
{
	int			count;
	char		*value;
	t_replace	rep;

	count = count_variable_length(*line, i);
	value = get_exit_variable_value(envp, *line + (*i - count), count + 1,
			last_exit_status);
	rep.start = (*i) - count - 1;
	rep.len = count + 1;
	rep.value = value;
	rep.i = i;
	replace_exit_status(line, &rep);
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
			&& (*line)[i] == '$' && (*line)[i + 1] == '?')
			extract_and_replace_exit_status(&i, line, envp, last_exit_status);
		else
			i++;
	}
}
