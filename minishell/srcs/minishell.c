/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:56:43 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 14:20:57 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char **convert_tokens_to_argv(t_token *tokens, int token_count)
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
/*static void process_command(t_token *tokens)
{
    int token_count;
    char **argv;

    token_count = count_tokens(tokens);
    argv = convert_tokens_to_argv(tokens, token_count);
    if (tokens && strcmp(tokens->value, "echo") == 0)
    {
        my_echo(token_count, argv);
        free(argv);
    }

}*/

static void process_command(t_token *tokens)
{
    int token_count;
    char **argv;
    t_token *current = tokens;
    t_minishell *shell;

     (void)shell;
    // Save original stdin and stdout
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    token_count = count_tokens(tokens);
    argv = convert_tokens_to_argv(tokens, token_count);

    // Handle redirections before executing the command
    while (current)
    {
        if (current->type == TOKEN_REDIRECT)
        {
            if (!current->next)
            {
                perror("Syntax error: missing file for redirection");
                free(argv);
                return;
            }

            char *redir_symbol = current->value;
            char *filename = current->next->value;

            if (redir_symbol[0] == '>' && redir_symbol[1] == '>')  // Handle '>>'
            {
                if (redirect_output_append(filename) == -1)
                    break;
            }
            else if (redir_symbol[0] == '>')  // Handle '>'
            {
                if (redirect_output(filename) == -1)
                    break;
            }
            else if (redir_symbol[0] == '<')  // Handle '<'
            {
                if (redirect_input(filename) == -1)
                    break;
            }

            current = current->next;  // Skip the filename token
        }
        current = current->next;
    }
    // Execute commands after handling redirections
    if (tokens && strcmp(tokens->value, "echo") == 0)
    {
        my_echo(token_count, argv);
    }
    else
    {
        exec_extercmds(argv);
    }
    /*else
    {
        exit_code(shell, &argv[0], argv);
    }*/

    // Restore original stdin and stdout
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);

    free(argv);
}

void initialize_shell(t_minishell *shell, int argc, char **argv)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);

    shell->environment = get_environment();
    (void)argv;
    if (argc != 1)
    {
        printf("Usage: ./minishell to enter minishell\n");
        return ;
    }
    print_banner();

}
void    handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

int main(int argc, char **argv)
{
    t_minishell shell;
    char *line;
    t_token *tokens;
    (void)shell;

    initialize_shell(&shell, argc, argv);
    shell.last_exit_status = 0;
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break;
        if (*line)
            add_history(line);
        tokens = tokenize_input(line);
        process_command(tokens);
        //print_list(tokens);
        free_tokens(tokens);
        free(line);
    }
    return 0;
}

/*int main(int argc, char **argv)
{
    t_minishell shell;
    char *line;

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);

    shell.environment = get_environment();
    (void)argv;
    if (argc != 1)
    {
        printf("Usage: ./minishell to enter minishell\n");
        return (1);
    }
    print_banner();
    while (1)
    {
        my_echo(argc, argv);
        line = readline("minishell$ ");
        if (!line)
            break;
        if (*line)
            add_history(line);
        t_token *tokens = tokenize_input(line);
        if (tokens && strcmp(tokens->value, "echo") == 0)
            execute_echo(tokens);
        free_tokens(tokens);
        free(line);
    }
    return 0;
}*/