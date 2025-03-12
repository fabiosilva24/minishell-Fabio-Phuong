/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:34:03 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/12 20:44:32 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_getpid(void)
{
	char	buffer[256];
	int		fd;
	int		bytes;
	int		pid;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("open", fd);
		return (-1);
	}
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes > 0)
	{
		buffer[bytes] = '\0';
		pid = ft_atoi(buffer);
		return (pid);
	}
	return (-1);
}
