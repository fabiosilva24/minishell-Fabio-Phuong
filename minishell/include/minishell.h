/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:56:36 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 12:03:59 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define MAX_PATH_LEN 4096
#define PWD_ERROR "minishell: pwd: error retrieving current directory\n"
#define EXIT_NUM_ERROR "exit: numeric argument required\n"
#define EXIT_MANY_ARGS "exit: too many arguments\n"
#define EXIT_MSG "exit\n"
#define ENV_ERROR "minishell: env: error retrieving environment variables\n"

# include <readline/history.h>       // add_history
# include <readline/readline.h>     // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <stdio.h>                 // printf, perror
# include <stdlib.h>                // malloc, free, exit
# include <unistd.h>                // write, access, fork, getcwd, chdir, unlink, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <fcntl.h>                 // open
# include <sys/types.h>             // fork, wait, waitpid, wait3, wait4, stat, lstat, fstat
# include <sys/stat.h>              // stat, lstat, fstat
# include <sys/wait.h>              // wait, waitpid, wait3, wait4
# include <signal.h>                // signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>                // opendir, readdir, closedir
# include <string.h>                // strerror
# include <termios.h>               // tcsetattr, tcgetattr
# include <curses.h>   // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>             // ioctl
# include <errno.h>                 // strerror (alternative for error strings)
# include "../libs/libft/libft.h"


extern char **environ;

typedef enum e_token_type
{
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_PIPE,
    TOKEN_REDIRECT,
  
}e_token_type;

typedef struct s_token
{
    char            *value;
    e_token_type    type;
    struct s_token  *next;
}t_token;

typedef struct s_minishell
{
    char            **environment;
    char            **envp;
    t_token         *token_list;
    int		last_exit_status;
}t_minishell;

typedef struct s_cmd
{
    char            **args;
} t_cmd;


void	print_banner();
void    double_quotes(char *symbol);
char    **get_environment(void);
void    single_quotes(char *symbol);
void    parse_input(char *input);
void exit_code(t_minishell *minishell, char **command, char **argv);
int     handle_redirection(char **arg);
int     redirect_input(const char *filename);
int     redirect_output(const char *filename);
int     redirect_output_append(const char *filename);
void exec_extercmds(char **argv);

//tokens

t_token *create_token(char *value, e_token_type type);
void    *free_tokens(t_token *tokens);
t_token *tokenize_input(char *line);
char    *ft_strtok(char *str, const char *delimeter);
int     count_tokens(t_token *tokens);

//handle ctrl cs
void    handle_sigint(int sig);



//built ins
void    my_echo(int argc, char **argv);
void    execute_echo(t_token *tokens);

//print to test
void print_list(t_token *current);

#endif
