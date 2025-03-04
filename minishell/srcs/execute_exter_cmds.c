#include "../include/minishell.h"

void exec_extercmds(char **argv, t_minishell *shell)
{
    pid_t pid;
    int status;
    char *path_env;
    char *path;
    char full_path[1024];
    char path_env_copy[MAX_PATH_LEN];

    int i = 0;
    while (argv[i])
    {
        printf("test argv: %s\n", argv[i++]);
    }
    
    path_env = getenv("PATH");
    //copy the path environment variable instead of calling getenv
    if (path_env)
    {
        ft_strncpy(path_env_copy, path_env, sizeof(path_env_copy));
    }
    else
    {
        perror("Error: PATH Environment Variable not found");
    }
    path = ft_strtok(path_env_copy, ":");

    pid = fork();
    if (pid == 0)
    {
        // Search for the executable in the PATH directories
        while (path != NULL)
        {
            ft_strncpy(full_path, path, sizeof(full_path));
            ft_strlcat(full_path, "/", sizeof(full_path));
            ft_strlcat(full_path, argv[0], sizeof(full_path));
            //snprintf(full_path, sizeof(full_path), "%s/%s", path, argv[0]);
            if (execve(full_path, argv, shell->environment) != -1)
            {
                break;
            }
            path = ft_strtok(NULL, ":");
        }

        // If execve fails for all paths
        perror("Error with execve");
        ft_putstr_fd("Error: command not found\n", 2);
        ft_putstr_fd("Error minishell: command not found\n", 2);
        shell->exit_status = 127;
        exit(127);
    }
    else if (pid < 0)
    {
        perror("error with fork");
        shell->exit_status = 127;
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            shell->exit_status = WEXITSTATUS(status);
        }
        else
        {
            shell->exit_status = 127;
        }
    }
}