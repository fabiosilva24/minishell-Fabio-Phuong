/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_exter_cmds.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsilva-p <fsilva-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:25:26 by fsilva-p          #+#    #+#             */
/*   Updated: 2025/03/07 15:25:26 by fsilva-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*void exec_extercmds(char **argv, t_minishell *shell)
{
    pid_t pid;
    int status;
    char *path_env;
    char *path;
    char full_path[1024];
    char path_env_copy[MAX_PATH_LEN];
    
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
}*/

void exec_extercmds(char **argv, t_minishell *shell)
{
    pid_t pid;
    int status;
    char *path_env;
    char *path;
    char full_path[1024];
    char path_env_copy[MAX_PATH_LEN];
    int cmd_found = 0;
    
    // Skip empty commands
    if (!argv || !argv[0])
        return;
    
    // Filter out redirection tokens from argv
    int i = 0;
    int j = 0;
    while (argv[i])
    {
        // Skip redirection operators and their arguments
        if ((strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0 ||
             strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0 || strcmp(argv[i], "|") == 0) && argv[i+1])
        {
            i += 2; // Skip the operator and its argument
            continue;
        }
        argv[j++] = argv[i++];
    }
    argv[j] = NULL; // Terminate the filtered array
    
    if (!argv[0]) // Check if we're left with any command after filtering
        return;
    
    path_env = getenv("PATH");
    if (!path_env)
    {
        perror("Error: PATH Environment Variable not found");
        return;
    }
    
    ft_strncpy(path_env_copy, path_env, sizeof(path_env_copy));
    path = ft_strtok(path_env_copy, ":");

    pid = fork();
    if (pid == 0) // Child process
    {
        // Try direct execution if it contains a path
        if (ft_strchr(argv[0], '/'))
        {
            if (access(argv[0], X_OK) == 0)
            {
                execve(argv[0], argv, shell->environment);
            }
            perror("Error accessing command");
            exit(127);
        }
        
        // Search PATH for the executable
        while (path != NULL)
        {
            ft_strncpy(full_path, path, sizeof(full_path));
            ft_strlcat(full_path, "/", sizeof(full_path));
            ft_strlcat(full_path, argv[0], sizeof(full_path));
            
            if (access(full_path, X_OK) == 0)
            {
                cmd_found = 1;
                execve(full_path, argv, shell->environment);
                break;
            }
            path = ft_strtok(NULL, ":");
        }

        // If command wasn't found or execution failed
        if (!cmd_found)
        {
            ft_putstr_fd("miaoushell: command not found: ", 2);
            ft_putstr_fd(argv[0], 2);
            ft_putstr_fd("\n", 2);
        }
        else
        {
            perror("Error with execve");
        }
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

