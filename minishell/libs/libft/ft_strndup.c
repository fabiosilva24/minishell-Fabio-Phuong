#include "libft.h"

char *ft_strndup(const char *s1, size_t n)
{
    char *copy;
    size_t len = ft_strlen(s1);

    if (len > n)
        len = n;
    copy = (char *)malloc(len + 1);
    if (!copy)
        return NULL;
    ft_strncpy(copy, s1, len);
    copy[len] = '\0';
    return copy;
}