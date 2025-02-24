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

# define MAX_PATH_LEN 4096
# define EXIT_NUM_ERROR "exit: numeric argument required\n"
# define EXIT_MANY_ARGS "exit: too many arguments\n"
# define EXIT_MSG "exit\n"
# define ENV_ERROR "minishell: env: error retrieving environment variables\n"
# define PWD_ERROR "minishell: pwd: error retrieving current directory\n"

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
    int             status;
    int             exit_status;
    t_token         *token_list;
    int		last_exit_status;
}t_minishell;

typedef struct s_cmd
{
    char            **args;
} t_cmd;

typedef struct s_path
{
    char    **dirs;
}   t_path;


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

//exec.c
int	is_builtin(t_cmd *cmd);
int	exec_process(t_cmd *cmd, char **envp);
int exec_cmd(t_cmd *cmd, t_minishell *shell);
void wait_for_all_processes(void);

//exec2.c
int   handle_builtin(t_cmd *cmd, t_minishell *shell);
void	handle_exit_status(int status, t_minishell *shell);

//exec_utils.c
void	print_cmd_error(const char *cmd, const char *message);
int	  check_cmd_validity(t_cmd *cmd);
int	  handle_execve_error(char *cmd_path, char *cmd);

//exec_path.c
char	**get_path_dirs(char **envp);
char *find_cmd_path(char **path_dirs, char *cmd);
char	*get_exec_path(t_cmd *cmd, char **envp);

//cmd_builtins_pwd_echo.c
void builtin_pwd(int *status);
void builtin_echo(t_cmd *cmd, int *status);

//cmd_builtins_env_exit.c
void builtin_env(char **envp, int *status);
void builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit);

//cmd_unset.c
char	**remove_variable_from_env(char *var_name, char **envp);
int     is_variable_set(char *var_name, char **envp);
char **builtin_unset(char **args, char **envp, int *status);

//cmd_builtins_export.c
char **extract_variable_names(char **envp);
char **prepare_sorted_env(char **envp);
int     prepare_export_data(char **tmpmass, char ***before, char ***after);
void    print_export_without_args(char **envp);
char **builtin_export(char **args, char **envp);

//export_utils.c
char	*get_variable_name(char *env_entry);
void	free_variable_names(char **variable_names, int i);
int	  print_env_entry(char *before, char *after, char *entry);
int	  is_valid_identifier(char *str);
void	update_envp(char *new_value, char *arg, char ***envp);

//env_management.c
char **add_to_envp(char **envp, char *str, int should_free);
int     find_and_replace(char **envp, char *key, char *new_value, size_t key_len);
char **replace_env_var(char **envp, char *key, char *new_value);

//env_management2.c
char *find_env_var(char *args, char **envp);
char **sort_env_vars(char **envp);
char **remove_quotes_from_env(char **envp);

//cd.c
void validate_and_cd(char *path, char *args);
void builtin_cd(char **args, char ***envp);

//cd_utils.c
char *get_cd_env_value(char **envp, char *key);
void update_pwd_variables(char ***envp, char *old_pwd);
void handle_cd_error(char *path);
void handle_absolute_path(char *path);
void handle_relative_path(char **args, char **envp);

//utils2.c
void	free_tab(char **tab);
int	  errmsg(char *s1, char *s2, char *s3, int code);
int	  size_mass(char **envp);
char	**new_envp(char **envp);
int   ft_sym_export(char *str);

#endif
