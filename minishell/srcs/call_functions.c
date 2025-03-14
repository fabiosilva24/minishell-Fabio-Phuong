/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:21:44 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 20:30:51 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    free_line_andtoken(t_token *tokens, char *line, t_minishell *shell)
{
    (void)shell;
    free_tokens(tokens);
    free(line);
}