/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42luxembourg.>  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/14 15:52:49 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 20:15:36 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_token(t_token **current, t_minishell *shell,
		t_token *tokens)
{
	if ((*current)->type == TOKEN_PIPE)
	{
		process_pipes(tokens, shell);
		return (1);
	}
	else if ((*current)->type == TOKEN_REDIRECT)
	{
		handle_redirection_error(*current);
		if (setup_redirection(*current) == -1)
			return (1);
		*current = (*current)->next;
	}
	return (0);
}

static void	process_command(t_token *tokens, t_minishell *shell)
{
	t_token	*current;
	int		token_count;
	int		original_stdin;
	int		original_stdout;

	current = tokens;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	token_count = count_tokens(tokens);
	while (current)
	{
		if (process_token(&current, shell, tokens))
			break ;
		current = current->next;
	}
	if (current == NULL)
		execute_command(tokens, shell, token_count);
	restore_io(original_stdin, original_stdout);
}

void	initialize_shell(t_minishell *shell, int argc, char **argv)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell->environment = get_environment();
	shell->exit_status = 0;
	shell->status = 0;
	(void)argv;
	if (argc != 1)
	{
		printf("Usage: ./minishell to enter minishell\n");
		return ;
	}
	print_banner();
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv)
{
	t_minishell	shell;
	char		*line;
	t_token		*tokens;

	initialize_shell(&shell, argc, argv);
	while (1)
	{
		line = readline("\001\033[1;32m\002✨miaoushell > \001\033[0m\002");
		if (!line)
		{
			ft_exit(&shell);
			break ;
		}
		line_history(line);
		tokens = tokenize_input(line);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		process_command(tokens, &shell);
		free_line_andtoken(tokens, line, &shell);
	}
	rl_clear_history();
	return (0);
}
