#include "../include/minishell.h"

void exec_extercmds(char **argv, t_minishell *shell)
{
    pid_t pid;
    int status;
    char *path_env = getenv("PATH");
    char *path;
    char full_path[1024];

    path_env = getenv("PATH");
    path = strtok(path_env, ":");

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
            path = strtok(NULL, ":");
        }

        // If execve fails for all paths
        perror("Error with execve");
        ft_putstr_fd("Error: command not found\n", 2);
        ft_putstr_fd("Error minishell: command not found\n", 2);
        shell->exit_status = 127;
        exit(1);
    }
    else if (pid < 0)
    {
        perror("error with fork");
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}