/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:11:36 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 19:21:39 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_output_redirection(char **arg)
{
	char	*filename;
	int		is_append;

	is_append = 0;
	(*arg)++;
	if (**arg == '>')
	{
		is_append = 1;
		(*arg)++;
	}
	while (**arg == ' ')
		(*arg)++;
	filename = *arg;
	if (*filename == '\0')
		return (-1);
	if (is_append)
		redirect_output_append(filename);
	else
		redirect_output(filename);
	return (1);
}

static int	handle_input_redirection(char **arg)
{
	char	*filename;
	int		is_heredoc;

	is_heredoc = 0;
	(*arg)++;
	if (**arg == '<')
	{
		is_heredoc = 1;
		(*arg)++;
	}
	while (**arg == ' ')
		(*arg)++;
	filename = *arg;
	if (*filename == '\0')
		return (-1);
	if (is_heredoc)
		handle_heredoc(filename);
	else
		redirect_input(filename);
	return (1);
}

int	handle_redirection(char **arg)
{
	if (**arg == '>')
		return (handle_output_redirection(arg));
	else if (**arg == '<')
		return (handle_input_redirection(arg));
	return (0);
}
