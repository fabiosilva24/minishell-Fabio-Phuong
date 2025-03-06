#include "../include/minishell.h"

static void execute_piped_command(t_token *cmd_tokens, t_minishell *shell, int pipe_in, int pipe_out)
{
    pid_t   pid;
    int     token_count;
    char    **arg;
    t_cmd   cmd;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return ;
    }

    if (pid == 0) //Child process
    {
        if (pipe_in != STDIN_FILENO)
        {
            dup2(pipe_in, STDIN_FILENO);
            close(pipe_in);
        }
        if (pipe_out != STDOUT_FILENO)
        {
            dup2(pipe_out, STDOUT_FILENO);
            close(pipe_out);
        }
        token_count = count_tokens(cmd_tokens);
        arg = convert_tokens_to_argv(cmd_tokens, token_count);
        cmd.args = arg;
        handle_redirection(arg);
        if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
            exec_extercmds(arg, shell);
        exit(shell->exit_status);
    }
}

void process_pipes(t_token *tokens, t_minishell *shell)
{
    int pipe_fd[2];
    int prev_pipe;
    t_token *cmd_start = tokens;
    t_token *current = tokens;
    int status;
    pid_t last_pid;

    int original_stdin;
    int original_stdout;
    int token_count;
    char **arg;
    t_cmd cmd;

    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    prev_pipe = STDIN_FILENO;
    
    while (current)
    {
        while (current && !(current->type == TOKEN_PIPE))
            current = current->next;

        if (current) // if pipe is found create a new one
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return ;
            }
            execute_piped_command(cmd_start, shell, prev_pipe, pipe_fd[1]);
            close(pipe_fd[1]);
            if (prev_pipe != STDIN_FILENO)
                close(prev_pipe);
            prev_pipe = pipe_fd[0];
            cmd_start = current->next;//MOVE
            current = current->next;//MOVE
        }
        else
        {
            last_pid = fork();
            if (last_pid == 0)
            {
                if (prev_pipe != STDERR_FILENO)
                {
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }
                token_count = count_tokens(cmd_start);
                arg = convert_tokens_to_argv(cmd_start, token_count);
                cmd.args = arg;
                handle_redirection(arg);
                if (exec_builtins(&cmd, &(shell->environment), shell) == 0)
                    exec_extercmds(arg, shell);
                exit(shell->exit_status);
            }
        if (prev_pipe != STDERR_FILENO)
            close(prev_pipe);
        waitpid(last_pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        }
    }
    //RESTORE
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}