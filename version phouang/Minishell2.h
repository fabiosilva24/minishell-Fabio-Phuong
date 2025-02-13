#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# define MAX_PATH_LEN 4096
# define EXIT_NUM_ERROR "exit: numeric argument required\n"
# define EXIT_MANY_ARGS "exit: too many arguments\n"
# define EXIT_MSG "exit\n"
# define ENV_ERROR "minishell: env: error retrieving environment variables\n"
# define PWD_ERROR "minishell: pwd: error retrieving current directory\n"

typedef struct s_minishell
{
    char    **envp;
    int     exit_status;
    int     status;
}   t_minishell;

typedef struct s_cmd
{
    char    **args;
}   t_cmd;

typedef struct s_path
{
    char    **dirs;
}   t_path;

// exec cmd
int     exec_process(t_cmd *cmd, char **envp);
int     exec_cmd(t_cmd *cmd, t_minishell *shell);
int     is_builtin(t_cmd *cmd);
int     handle_builtin(t_cmd *cmd, t_minishell *shell);
void    wait_for_all_processes(void);
void    handle_exit_status(int status, t_minishell *shell);

// exec utils
void    free_tab(char **tab);
int     check_cmd_validity(t_cmd *cmd);
int     handle_execve_error(char *cmd_path, char *cmd);
void    print_cmd_error(const char *cmd, const char *message);

// exec path
char    **get_path_dirs(char **envp);
char    *get_exec_path(t_cmd *cmd, char **envp);
char    *find_cmd_path(char **path_dirs, char *cmd);

// builtins
void    builtin_env(char **envp, int *status);
void    builtin_exit(t_cmd *cmd, t_minishell *shell, int should_exit);
void    builtin_pwd(int *status);
void    builtin_echo(t_cmd *cmd, int *status);
int     builtin_cd(t_cmd *cmd, t_minishell *shell);
int     builtin_export(t_cmd *cmd, t_minishell *shell);
int     builtin_unset(t_cmd *cmd, t_minishell *shell);

char    *get_env_var_value(t_minishell *shell, char *line, int len);
int     sym_export(t_minishell *shell, char *var);
int     ft_setenv(t_minishell *shell, char *key, char *value);

#endif
