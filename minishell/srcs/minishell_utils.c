/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:12:44 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/15 04:14:40 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_exit(t_minishell *shell)
{
	printf("exit\n");
	cleanup_shell(shell);
}
void    line_history(char *line)
{
	if (*line)
		add_history(line);
}
