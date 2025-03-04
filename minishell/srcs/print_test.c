#include "../include/minishell.h"

void print_list(t_token *current)
{
	printf("Linked list: ");
	while (current)
	{
		printf(" %s -> ", current->value);
		current = current->next;
	}
	printf("\n");
}