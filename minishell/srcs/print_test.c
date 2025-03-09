/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 17:00:10 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/09 17:00:10 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_list(t_token *current)
{
	printf("Linked list: ");
	while (current)
	{
		printf(" %s -> ", current->value);
		current = current->next;
	}
	printf("\n");
}
