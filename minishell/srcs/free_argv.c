/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:49:04 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/10 21:46:02 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_argv(char **argv)
{
    int i;

    if (!argv)
        return;
    i = 0;
    while (argv[i])
    {
        if (argv[i] == NULL)
        {
            argv[i] = NULL;
            return ;
        }
        free(argv[i]);
        argv[i] = NULL;
        i++;
    }
    free(argv);
    argv = NULL;
}