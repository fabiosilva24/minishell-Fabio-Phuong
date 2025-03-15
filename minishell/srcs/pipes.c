/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42luxembourg.>  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/14 15:52:49 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/14 16:03:30 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	is_redirection(char *token)
{
	return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 || strcmp(token,
			"<") == 0 || strcmp(token, "<<") == 0);
}

int	count_tokens_until_pipe(t_token *tokens)
{
	int count = 0;
	t_token *current = tokens;

	while (current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}

	return (count);
}

char	**convert_tokens_to_argv_until_pipe(t_token *tokens, int token_count)
{
	char **argv;
	int i = 0;
	t_token *current;

	if (!tokens || token_count <= 0)
		return (NULL);
	current = tokens;
	argv = malloc(sizeof(char *) * (token_count + 1));
	if (!argv)
		return (NULL);
	while (current && i < token_count && current->type != TOKEN_PIPE)
	{
		argv[i] = ft_strdup(current->value);
		i++;
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

void process_pipes(t_token *tokens, t_minishell *shell)
{
    int pipe_fd[2];
    int prev_pipe = STDIN_FILENO;
    t_token *cmd_start = tokens;
    t_token *current = tokens;
    pid_t pids[1024]; // Store child PIDs
    int pid_count = 0;
    int status;
    int i;
    int original_stdin;
    int original_stdout;

    original_stdout = dup(STDOUT_FILENO);
    original_stdin = dup(STDIN_FILENO);
    i = 0;

    while (current)
    {
        // Find the next pipe token
        t_token *pipe_token = cmd_start;
        while (pipe_token && pipe_token->type != TOKEN_PIPE)
            pipe_token = pipe_token->next;

        if (pipe_token) // There is a pipe, so there's another command
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return;
            }

            // Debug print to see the command being processed
            printf("Forking process for command: %s\n", cmd_start->value);

            pids[pid_count] = fork();
            if (pids[pid_count] == -1)
            {
                perror("fork");
                return;
            }
            if (pids[pid_count] == 0) // Child process
            {
                // Redirect input if necessary
                if (prev_pipe != STDIN_FILENO)
                {
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }

                // Redirect output to pipe_fd[1]
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
                close(pipe_fd[0]); // Close the unused read end

                // Execute the command
                execute_piped_command(cmd_start, shell, prev_pipe, pipe_fd[1]);
                exit(shell->exit_status);
            }

            // Parent process
            pid_count++;
            printf("Closing write end of pipe_fd[1] in parent\n");
            close(pipe_fd[1]); // Close the write end in the parent
            if (prev_pipe != STDIN_FILENO)
                close(prev_pipe); // Close the previous read end
            prev_pipe = pipe_fd[0];

            // Move to the next command in the pipeline
            cmd_start = pipe_token->next;
            current = cmd_start;
        }
        else // Last command, no more pipes
        {
            // Debug print to see the last command being processed
            printf("Executing last command: %s\n", cmd_start->value);

            pids[pid_count] = fork();
            if (pids[pid_count] == -1)
            {
                perror("fork");
                return;
            }
            if (pids[pid_count] == 0) // Child process
            {
                // Redirect input if necessary
                if (prev_pipe != STDIN_FILENO)
                {
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }

                // Execute the last command
                execute_piped_command(cmd_start, shell, prev_pipe, STDOUT_FILENO);
                exit(shell->exit_status);
            }

            // Parent process
            pid_count++;
            if (prev_pipe != STDIN_FILENO)
                close(prev_pipe); // Close previous pipe in the parent
            break; // We don't need to process more commands
        }
    }

    // Wait for all child processes
    while (i < pid_count)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        i++;
    }

    // Restore the original stdin and stdout
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}



