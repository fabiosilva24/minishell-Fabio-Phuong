/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/01/30 17:56:36 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/02/07 12:03:59 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_PATH_LEN 4096
# define EXIT_NUM_ERROR "exit: numeric argument required\n"
# define EXIT_MANY_ARGS "exit: too many arguments\n"
# define EXIT_MSG "exit\n"
# define TMP_FILE ".heredoc_tmp"

# include "../libs/libft/libft.h"
# include <curses.h>            // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
# include <dirent.h>            // opendir, readdir, closedir
# include <errno.h>             // strerror (alternative for error strings)
# include <fcntl.h>             // open
# include <readline/history.h>  // add_history
# include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line,
# include <signal.h>            // signal, sigaction, sigemptyset, sigaddset,
# include <stdio.h>             // printf, perror
# include <stdlib.h>            // malloc, free, exit
# include <string.h>            // strerror
# include <sys/ioctl.h>         // ioctl
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/types.h>         // fork, wait, waitpid, wait3, wait4, stat,
# include <sys/wait.h>          // wait, waitpid, wait3, wait4
# include <termios.h>           // tcsetattr, tcgetattr
# include <unistd.h>            // write, access, fork, getcwd, chdir, unlink,
# include <ctype.h>

extern char **environ;

typedef enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_PIPE,
	TOKEN_REDIRECT,
}   e_token_type;

typedef struct s_token
{
	char *value;
	e_token_type type;
	struct s_token *next;
} t_token;

typedef struct s_minishell
{
	char **environment;
	char **envp;
	int status;
	int exit_status;
	t_token *token_list;
} t_minishell;

typedef struct s_cmd
{
	char **args;
} t_cmd;

typedef struct s_path
{
	char **dirs;
} t_path;

typedef struct s_error
{
	const char	*s1;
	const char	*s2;
	const char	*s3;
	int			code;
	int			*status;
}	t_error;

typedef struct s_cd
{
	char	*path;
	char	*args;
	char	*old_path;
	char	*str;
	int		j;
	int		i;
	int		*status;
}	t_cd;

typedef struct s_export
{
	int		i;
	int		j;
	char	*new_value;
	char	**env_copy;
	char	**names;
	char	**values;
}	t_export;

typedef struct s_replace
{
	int		start;
	int		len;
	char	*value;
	int		*i;
}	t_replace;

void	print_banner(void);
void	double_quotes(char *symbol, t_minishell *shell);
char	**get_environment(void);
void	single_quotes(char *symbol);
void	parse_input(char *input, t_minishell *shell);
void	exit_code(t_minishell *minishell, char **command, char **argv);
int		handle_redirection(char **arg);
int		redirect_input(const char *filename);
int		redirect_output(const char *filename);
int		redirect_output_append(const char *filename);
int		redirect_heredoc_input(void);
void	exec_extercmds(char **argv, t_minishell *shell);
int		handle_heredoc(const char *delimiter);
// tokens

t_token	*create_token(char *value, e_token_type type);
void	*free_tokens(t_token *tokens);
t_token	*tokenize_input(char *line);
char	*ft_strtok(char *str, const char *delimeter);
int		count_tokens(t_token *tokens);

// handle ctrl cs
void	handle_sigint(int sig);
void	setup_signal_handler(t_minishell *shell);
// handle exit code for ctrlc;
void	handle_sigint_with_shell(int sig, siginfo_t *info, void *context);
// handle exit code for ctrlc;

// built ins
void	my_echo(int argc, char **argv);
void	execute_echo(t_token *tokens);
void	handle_dollarsign(char *symbol, int *i, int len, t_minishell *shell);

// print to test
void	print_list(t_token *current);

// cd.c
char	*get_after_char(const char *s, int c);
char	**update_pwd_oldpwd(char **envp, char *old_path, int *status);
char	**change_directory(char **args, int if_cdcmd, char **envp,
		int *status);

// cd_utils.c
void	handle_cd_status(t_cd *cd);
void	check_directory_exists(t_cd *cd);
void	process_cd_path(char *path, char *old_path, int is_cd_builtin,
		int *status);

// cmd_builtins_echo_env_pwd.c
void	builtin_echo(t_cmd *cmd, int *status, t_minishell *shell);
void	builtin_pwd(int *status);
void	builtin_env(char **envp, int *status);

// cmd_builtins_exit.c
void	builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit);
void	handle_exit_with_arg(t_cmd *cmd, t_minishell *shell,
		int should_exit);
// cmd_builtins_export.c
char	**add_env_var(char **envp, char *str, int free_old);
char	*find_double_var(char *args, char **envp);
char	**replace_env_var(char **envp, char *args, char *new);
char	**sort_env_vars(char **mass);
char	**extract_var_values(char **tmpmass);

// cmd_builtins_export_utils.c
char	**extract_var_names(char **tmpmass);
void	print_sorted_env(char **envp, int *status);
char	**change_envp(char *new, char **args, int i, char **envp);
char	**builtin_export(char **args, char **envp, int *status);

// cmd_builtins_unset.c
char	**remove_env_var(char *args, char **envp);
int		is_env_var_present(char *args, char **envp);
void	builtin_unset(char **args, char ***envp, int *status);

// exec.c
char	**extract_path_directories(char **envp);
char	*find_executable(char **paths, char **cmd_flags, int *status);
int		exec_builtins(t_cmd *cmd, char ***envp, t_minishell *shell);
void	execute(t_cmd *cmd, char ***envp, t_minishell *shell);

// exit.c
char	*get_exit_variable_value(char **envp, char *line, int len,
		int last_exit_status);
void	replace_exit_status(char **line, t_replace *rep);
void	extract_and_replace_exit_status(int *i, char **line, char **envp,
		int last_exit_status);
void	expand_exit_status(char **line, char **envp, int last_exit_status);

// utils.c
void	errmsg(t_error *err);
int		size_mass(char **envp);
char	**new_envp(char **envp);
int		ft_sym_export(char *str);
void	ft_free(char **mass);
int		ft_len_eq(char *str);
int		max(int a, int b);
void	check_if_command_exists(t_cmd *cmd, t_minishell *shell);
char	*extract_var_name(char *arg);

void	process_pipes(t_token *tokens, t_minishell *shell);
char	**convert_tokens_to_argv(t_token *tokens, int token_count);
int		count_tokens_until_pipe(t_token *tokens);
char	**convert_tokens_to_argv_until_pipe(t_token *tokens, int token_count);


int 	apply_redirection(char *redir_symbol, char *filename);
int 	is_redirection(char *token);
void	extract_variable_name(const char *symbol, int *i, int len, char *var_name);
char	*find_env_value(const char *var_name, char **environment);

void	free_argv(char **argv);
int   ft_getpid();

#endif
