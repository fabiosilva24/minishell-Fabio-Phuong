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
# include <curses.h>        
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include <ctype.h>

extern char	**environ;

enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_PIPE,
	TOKEN_REDIRECT,
};

typedef struct s_token
{
	char			*value;
	e_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_minishell
{
	char	**environment;
	char	**envp;
	int		status;
	int		exit_status;
	t_token	*token_list;
}	t_minishell;

typedef struct s_cmd
{
	char	**args;
}	t_cmd;

typedef struct s_path
{
	char	**dirs;
}	t_path;

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

typedef struct s_replace_data
{
	int		j;
	char	*new;
}	t_replace_data;

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
void	exec_extercmds(char **argv, t_minishell *shell, t_token *tokens);
int		handle_heredoc(const char *delimiter);

t_token	*create_token(char *value, e_token_type type);
void	*free_tokens(t_token *tokens);
t_token	*tokenize_input(char *line);
char	*ft_strtok(char *str, const char *delimeter);
int		count_tokens(t_token *tokens);

void	handle_sigint(int sig);
void	setup_signal_handler(t_minishell *shell);

void	handle_sigint_with_shell(int sig, siginfo_t *info, void *context);

void	my_echo(int argc, char **argv);
void	execute_echo(t_token *tokens);
void	handle_dollarsign(char *symbol, int *i, int len, t_minishell *shell);

void	print_list(t_token *current);

char	*get_after_char(const char *s, int c);
char	**update_pwd_oldpwd(char **envp, char *old_path, int *status);
char	**change_directory(char **args, int if_cdcmd, char **envp,
			int *status);

void	handle_cd_status(t_cd *cd);
void	check_directory_exists(t_cd *cd);
int		search_directory(t_cd *cd);
void	process_cd_path(char *path, char *old_path, int is_cd_builtin,
			int *status);

void	builtin_echo(t_cmd *cmd, int *status, t_minishell *shell);
void	builtin_pwd(int *status);
void	builtin_env(char **envp, int *status);

void	builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit);
void	handle_exit_with_arg(t_cmd *cmd, t_minishell *shell,
			int should_exit);

char	**add_env_var(char **envp, char *str, int free_old);
char	*find_double_var(char *args, char **envp);
char	**replace_env_var(char **envp, char *args, char *new);
char	**sort_env_vars(char **mass);
char	**extract_var_values(char **tmpmass);

char	**extract_var_names(char **tmpmass);
void	print_sorted_env(char **envp, int *status);
char	**change_envp(char *new, char **args, int i, char **envp);
char	**builtin_export(char **args, char **envp, int *status);

char	**remove_env_var(char *args, char **envp);
int		is_env_var_present(char *args, char **envp);
void	builtin_unset(char **args, char ***envp, int *status);

char	**extract_path_directories(char **envp);
char	*find_executable(char **paths, char **cmd_flags, int *status);
int		exec_builtins(t_cmd *cmd, char ***envp, t_minishell *shell);
void	execute(t_cmd *cmd, char ***envp, t_minishell *shell);

char	*get_exit_variable_value(char **envp, char *line, int len,
			int last_exit_status);
void	replace_exit_status(char **line, t_replace *rep);
void	extract_and_replace_exit_status(int *i, char **line, char **envp,
			int last_exit_status);
void	expand_exit_status(char **line, char **envp, int last_exit_status);

void	errmsg(t_error *err);
int		size_mass(char **envp);
char	**new_envp(char **envp);
int		ft_sym_export(char *str);
void	ft_free(char **mass);
int		ft_len_eq(char *str);
int		max(int a, int b);
void	check_if_command_exists(t_cmd *cmd, t_minishell *shell);
char	*extract_var_name(char *arg);
void	free_envp(char **envp);
void	cleanup_shell(t_minishell *shell);

void	process_pipes(t_token *tokens, t_minishell *shell);
char	**convert_tokens_to_argv(t_token *tokens, int token_count);
int		count_tokens_until_pipe(t_token *tokens);
char	**convert_tokens_to_argv_until_pipe(t_token *tokens, int token_count);

int		apply_redirection(char *redir_symbol, char *filename);
int		is_redirection(char *token);
void	extract_variable_name(const char *symbol, int *i, int len,
			char *var_name);
char	*find_env_value(const char *var_name, char **environment);

void	free_argv(char **argv);
int		ft_getpid(void);

void	free_new_mass_on_error(char **new_mass, int i);
int		copy_env_var(char **new_mass, char **envp, int i, int j);
int		copy_or_replace_var(char **new_env, char **envp,
			char *args, t_replace_data *data);
int		copy_existing_vars(char **new_env, char **envp, int *j);
int		add_new_var(char **new_env, char *str, int j);
char	*create_oldpwd_var(char *old_path);
char	*create_pwd_var(void);
char	**update_env_vars(char **envp, char *pwd_var, char *oldpwd_var);
void	setup_pipe_io(int pipe_in, int pipe_out);
void	handle_redirectionss(char **arg, int token_count);
void	execute_child_process1(t_token *cmd_tokens, t_minishell *shell,
			int pipe_in, int pipe_out);
void	execute_piped_command(t_token *cmd_tokens, t_minishell *shell,
			int pipe_in, int pipe_out);
void	free_line_andtoken(t_token *tokens, char *line, t_minishell *shell);

#endif
