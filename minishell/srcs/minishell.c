/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/01/30 17:56:43 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 14:20:57 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	**convert_tokens_to_argv(t_token *tokens, int token_count)
{
	char **argv;
	int i;

	i = 0;
	argv = malloc((token_count + 1) * sizeof(char *));
	while (tokens)
	{
		argv[i++] = tokens->value;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}
static void	process_command(t_token *tokens, t_minishell *shell)
{
	int token_count;
	char **argv;
	t_token *current = tokens;

	//(void)shell;
	// Save original stdin and stdout
	int original_stdin = dup(STDIN_FILENO);
	int original_stdout = dup(STDOUT_FILENO);

	token_count = count_tokens(tokens);
	argv = convert_tokens_to_argv(tokens, token_count);

	// Handle redirections before executing the command
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			process_pipes(tokens, shell);
			free(argv);
			return ;
		}
		else if (current->type == TOKEN_REDIRECT)
		{
			if (!current->next)
			{
				perror("Syntax error: missing file for redirection");
				free(argv);
				return ;
			}

			char *redir_symbol = current->value;
			char *filename = current->next->value;

			if (apply_redirection(redir_symbol, filename) == -1)
				break;

			current = current->next; // Skip the filename token
		}
		else
		{
			current = current->next;
		}
	}

	// Execute commands after handling redirections
	t_cmd cmd;
	cmd.args = argv;
	// int exit_code = 0;
	// handle_dollar_questionmark(&cmd, &shell);
	if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
		exec_extercmds(argv, shell);

	// Restore original stdin and stdout
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);

	free(argv);
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
	t_minishell shell;
	char *line;
	t_token *tokens;

	initialize_shell(&shell, argc, argv);
	while (1)
	{
		line = readline("\001\033[1;32m\002✨miaoushell > \001\033[0m\002");
		if (!line)
        {
            printf("exit\n");
			break ;
        }
		if (*line)
			add_history(line);
		tokens = tokenize_input(line);
        if (!tokens)
        {
            free(line);
            continue;
        }
		process_command(tokens, &shell);
		print_list(tokens);
		free_tokens(tokens);
		free(line);
	}
	rl_clear_history();
	return (0);
}
