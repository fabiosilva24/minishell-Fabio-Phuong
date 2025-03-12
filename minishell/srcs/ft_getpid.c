#include "../include/minishell.h"

int   ft_getpid()
{
    int     fd;
    char    buffer[256];
    int     bytes;
    int     pid;

    fd = open("/proc/self/stat", O_RDONLY);
    if (fd < 0)
    {
        ft_putstr_fd("open", fd);
        return (-1);
    }
    bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        pid = ft_atoi(buffer);
        return (pid);
    }
    return (-1);
}