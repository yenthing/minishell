#include "../incl/minishell.h"

int	ft_check_before_equal(char *val)
{
	int	i;

	i = 0;
	if (val[i] && !ft_isalpha(val[i]))
		return (1);
	while (val[i])
	{
		if (val[i] == '=')
			return (0);
		if (!ft_isalnum(val[i]))
			return (1);
		i++;
	}
	return (1);
}

void	ft_repoint_env(t_env *tmp, t_env **new)
{
	if (tmp != *new)
		while (tmp->next != *new)
			tmp = tmp->next;
	if (tmp == *new)
	{
		free((*new)->str);
		(*new)->str = NULL;
	}
	else
	{
		tmp->next = (*new)->next;
		free ((*new)->str);
		free (*new);
		*new = NULL;
	}
}

int	ft_add_to_env(t_env **env, char *val)
{
	t_env	*new;
	t_env	*tmp;

	tmp = *env;
	new = ft_is_env(*env, val, 0);
	if (new)
		ft_repoint_env(tmp, &new);
	if (!new)
		new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->str = ft_strdup(val);
	if (!(new->str))
	{
		if (new != tmp)
			free(new);
		return (1);
	}
	if (tmp != new)
		new->next = *env;
	*env = new;
	return (0);
}

int	ft_check_after_equal(t_env *env, char *val)
{
	int	i;

	i = 0;
	(void)env;
	while (val[i])
	{
		if (val[i] == '=')
			break ;
		i++;
	}
	while (val[i])
	{
		if (val[i] == '!' && val[i + 1])
			return (ft_error(val, ": event not found\n", i));
		i++;
	}
	return (0);
}

int	ft_export(t_pipex *list, t_tokens *toks, int i, t_env *env)
{
	int		ret;

	toks = toks->next;
	ret = 0;
	if (!toks || toks->ind_command != i)
		return (ft_print_env_declare_x(env, list->redir_out));
	toks = ft_point_to_needed_tok(toks, i, 0, SEP);
	if (!toks || toks->ind_command != i)
		return (ft_print_env_declare_x(env, list->redir_out));
	ret = ft_check_before_equal(toks->val);
	if (ret)
		return (ft_export_error(toks, toks->val, i));
	ret = ft_check_after_equal(env, toks->val);
	if (!ft_last_pipe(toks, i))
		return (ret);
	if (!ret && !ft_strncmp(toks->val, "SHLVL=", 6))
		ft_export_shlvl(&(list->data->env), toks->val);
	else if (!ret)
	{
		if (!ft_strncmp(toks->val, "OLDPWD=", 7))
			list->data->old_pwd = 1;
		ft_add_to_env(&(list->data->env), toks->val);
	}
	return (ret);
}
