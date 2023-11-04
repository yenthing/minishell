#include "minishell.h"

void	ft_change_args(t_tokens **toks)
{
	t_tokens	*prev;
	t_tokens	*next;

	next = (*toks)->next;
	prev = (*toks)->prev;
	if ((*toks)->val)
		free((*toks)->val);
	free(*toks);
	if (prev)
	{
		*toks = prev;
		(*toks)->next = next;
		return;
	}
	*toks = next;
	(*toks)->prev = prev;
}

void	ft_newinfd(t_tokens **toks, t_pipex *list)
{
	int			y;
	char		*file;

	y = 3;
	while ((*toks)->type != COM)
		*toks = (*toks)->next;
	file = (*toks)->val;
	list->redir_in = open(file, O_RDONLY);
	if (list->redir_in < 0)
		perror(file);
	if ((*toks)->prev->type == SEP)
		++y;
	while (--y)
		ft_change_args(toks);
}

void	ft_newoutfd(t_tokens **toks, t_pipex *list)
{
	t_tokens	*rem_tok;
	int			y;
	char		*file;

	y = 3;
	rem_tok = *toks;
	while ((*toks)->type != COM)
		*toks = (*toks)->next;
	file = (*toks)->val;
	if (rem_tok->val[1] != '\0')
		list->redir_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0000644);
	else
		list->redir_out = open(file, O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (list->redir_out < 0)
		perror(file);
	if ((*toks)->prev->type == SEP)
		++y;
	while (--y)
		ft_change_args(toks);
}

void	ft_heredoc(t_tokens **toks, t_pipex *list)
{
	int			y;
	char		*delim;
	char		*buf;

	y = 3;
	while ((*toks)->type != COM)
		*toks = (*toks)->next;
	delim = (*toks)->val;
	list->redir_in = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (list->redir_in < 0)
		perror("heredoc");
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO, 0);
		if (!ft_strncmp(delim, buf, ft_strlen(delim)))
			break ;
		write(list->redir_in, buf, ft_strlen(buf));
		free(buf);
	}
	free(buf);
	get_next_line(-1, 1);
	close(list->redir_in);
	list->redir_in = open(".heredoc", O_RDONLY);
	if ((*toks)->prev->type == SEP)
		++y;
	while (--y)
		ft_change_args(toks);
}


// wc -l <test>>new
void	ft_redirects(int i, t_tokens **toks_orig, t_pipex *list)
{
	t_tokens *toks;

	toks = *toks_orig;  
	while (toks->ind_command != i)
		toks = toks->next;
	while (toks && toks->ind_command == i)
	{
		if (toks->type == REDIR_OUT)
			ft_newoutfd(&toks, list);
		if (toks->type == REDIR_IN)
			ft_newinfd(&toks, list);
		if (toks->type == HERE_DOC)
			ft_heredoc(&toks, list);
		toks = toks->next;
	}
}
