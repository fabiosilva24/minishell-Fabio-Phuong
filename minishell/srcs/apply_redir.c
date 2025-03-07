#include "../include/minishell.h"

int apply_redirection(char *redir_symbol, char *filename)
{
    if (redir_symbol[0] == '>' && redir_symbol[1] == '>')
    {
        return redirect_output_append(filename);
    }
    else if (redir_symbol[0] == '>')
    {
        return redirect_output(filename);
    }
    else if (redir_symbol[0] == '<' && redir_symbol[1] == '<')
    {
        return handle_heredoc(filename);
    }
    else if (redir_symbol[0] == '<')
    {
        return redirect_input(filename);
    }
    return 0;
}