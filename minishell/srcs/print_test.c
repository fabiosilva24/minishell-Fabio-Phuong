#include "../include/minishell.h"

void print_list(t_token *current)
{
	while (current)
	{
		printf("%s->", current->value);
		current = current->next;
	}
	printf("\n");
}