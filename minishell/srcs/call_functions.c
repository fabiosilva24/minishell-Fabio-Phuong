/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:21:44 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 17:52:46 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_line_andtoken(t_token *tokens, char *line, t_minishell *shell)
{
	if (tokens)
		free_tokens(tokens);
	if (line)
		free(line);
	(void)shell;
}
