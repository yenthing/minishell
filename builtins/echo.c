#include "../incl/minishell.h"

int	ft_end_space(t_tokens *toks, int i)
{
	if (toks->type != SEP)
		return (0);
	toks = toks->next;
	while (toks && toks->ind_command == i)
	{
		if (toks->type == COM)
			return (0);
		toks = toks->next;
	}
	return (1);
}

void	ft_echo_env(t_env *env, char *str, int *i, t_pipex *list)
{
	int		n;
	t_env	*to_print;

	n = *i;
	*i = *i + 1;
	while (str[++n])
	{
		if (n == *i && (str[n] == '?' || str[n] == '0'))
		{
			if (str[n] == '0')
				ft_putstr_fd("minishell", list->out);
			else
				ft_putnbr_fd(g_minishell, list->out);
			*i = *i + 1;
			return ;
		}
		if (!ft_isalpha(str[n]))
			break ;
	}
	if (n == *i)
		return ;
	to_print = ft_is_env(env, str + *i, n - *i);
	if (to_print)
		ft_putstr_fd(to_print->str, list->out);
	*i = n;
}

int	ft_quote_cond(char *str, int *i, int *meet_again, t_pipex *list)
{
	if (str[*i] == D || str[*i] == S)
	{
		if (!(*meet_again))
			*meet_again = str[*i];
		else
			*meet_again = 0;
	}
	if (*meet_again == S)
	{
		*i = *i + 1;
		while (str[*i] != S)
		{
			write(list->out, &str[*i], 1);
			*i = *i + 1;
		}
		*i = *i + 1;
		*meet_again = 0;
		return (1);
	}
	else if (str[*i] == D)
	{
		*i = *i + 1;
		return (1);
	}
	return (0);
}

int	ft_echo_normal(t_env *env, char *str, t_pipex *list)
{
	int	i;
	int	meet_again;

	i = 0;
	meet_again = 0;
	if (ft_error_screen(str))
		return (1);
	while (str[i])
	{
		if (ft_quote_cond(str, &i, &meet_again, list))
			;
		else
		{
			if (str[i] == '$' && str[i + 1]
				&& (ft_isalnum(str[i + 1]) == 1 || str[i + 1] == '?'))
				break ;
			write(list->out, &str[i], 1);
			i++;
		}
	}
	if (str[i])
		ft_echo_env(env, str, &i, list);
	if (str[i])
		ft_echo_normal(env, str + i, list);
	return (0);
}

int	ft_echo(t_pipex *list, t_tokens *toks, int i)
{
	int	err;

	err = 0;
	while (toks->ind_command != i)
		toks = toks->next;
	toks = toks->next;
	toks = ft_point_to_needed_tok(toks, i, 0, SEP);
	if (toks && toks->ind_command == i && toks->type == COM)
	{
		while (toks && toks->ind_command == i)
		{
			if (toks->prev && toks->type == SEP && toks->prev->type == SEP)
				;
			else if (!ft_end_space(toks, i))
				err = ft_echo_normal(list->data->env, toks->val, list);
			toks = toks->next;
		}
	}
	if (!err)
		write(list->out, "\n", 1);
	return (0);
}
