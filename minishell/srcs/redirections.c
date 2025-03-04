/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:26:22 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/09 15:26:22 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int redirect_heredoc_input()
{
	int fd;

	fd = open(TMP_FILE, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}

	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink(TMP_FILE);
	return (0);
}
int handle_heredoc(const char *delimeter)
{
	int fd;
	char *line;

	fd = open(TMP_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break;
		
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (redirect_heredoc_input());
}
int redirect_input(const char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening input file");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int redirect_output(const char *filename)
{
	int fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int redirect_output_append(const char *filename)
{
	int fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}