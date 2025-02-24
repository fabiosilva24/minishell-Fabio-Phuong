#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <dirent.h>

# define MAX_PATH_LEN 4096
# define EXIT_NUM_ERROR "exit: numeric argument required\n"
# define EXIT_MANY_ARGS "exit: too many arguments\n"
# define EXIT_MSG "exit\n"
# define ENV_ERROR "minishell: env: error retrieving environment variables\n"
# define PWD_ERROR "minishell: pwd: error retrieving current directory\n"

typedef struct s_minishell
{
    char    **envp;
    int     status;
    int     exit_status;
}   t_minishell;

typedef struct s_cmd
{
    char    **args;
}   t_cmd;

typedef struct s_path
{
    char    **dirs;
}   t_path;

//exec.c
int	is_builtin(t_cmd *cmd);
int	exec_process(t_cmd *cmd, char **envp);
int exec_cmd(t_cmd *cmd, t_minishell *shell);
void wait_for_all_processes(void);

//exec2.c
static int handle_builtin_part1(t_cmd *cmd, t_minishell *shell);
static int handle_builtin_part2(t_cmd *cmd, t_minishell *shell);
int   handle_builtin(t_cmd *cmd, t_minishell *shell);
void	handle_exit_status(int status, t_minishell *shell);

//exec_utils.c
void	print_cmd_error(const char *cmd, const char *message);
int	  check_cmd_validity(t_cmd *cmd);
int	  handle_execve_error(char *cmd_path, char *cmd);

//exec_path.c
char	**get_path_dirs(char **envp);
static int	   is_executable(const char *path);
static char    *try_direct_path(char *cmd);
char *find_cmd_path(char **path_dirs, char *cmd);
char	*get_exec_path(t_cmd *cmd, char **envp);

//cmd_builtins_pwd_echo.c
void builtin_pwd(int *status);
static int  has_n_flag(const char *arg);
static void print_echo_args(char **args, int start_idx, int *status);
void builtin_echo(t_cmd *cmd, int *status);

//cmd_builtins_env_exit.c
void builtin_env(char **envp, int *status);
static int is_numeric_arg(const char *arg);
void builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit);

//cmd_unset.c
static int copy_or_skip_var(char **new_envp, char **envp, char *var_name, int *i);
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
static char **copy_existing_env(char **envp, char **new_env);
char **add_to_envp(char **envp, char *str, int should_free);
static char *create_new_entry(char *key, char *new_value);
int     find_and_replace(char **envp, char *key, char *new_value, size_t key_len);
char **replace_env_var(char **envp, char *key, char *new_value);

//env_management2.c
char *find_env_var(char *args, char **envp);
static void swap_strings(char **str1, char **str2);
static void bubble_sort_iteration(char **envp, int size, int i);
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
static char *resolve_home_path(char *arg, char **envp);
void handle_relative_path(char **args, char **envp);

//utils2.c
void	free_tab(char **tab);
int	  errmsg(char *s1, char *s2, char *s3, int code);
int	  size_mass(char **envp);
char	**new_envp(char **envp);
int   ft_sym_export(char *str);

#endif
