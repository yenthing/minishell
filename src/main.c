
#include "../incl/minishell.h"

int		g_minishell;

char	**malloc_input(char **av, int ac)
{
	char	**input;
	int		i;

	i = 1;
	input = NULL;
	input = ft_calloc(ac + 2, sizeof(char *));
	input[0] = malloc(12 * sizeof(char));
	input[0] = "./minishell\0";
	while (i <= ac)
	{
		input[i] = av[i - 1];
		i++;
	}
	input[i] = NULL;
	return (input);
}

// if input i == -1 -> return ac
int		ft_find_tok(t_tokens *toks, int i)
{
	while(toks && toks->next)
		toks = toks->next;
	if (i == -1)
		return (toks->ind_command + 1);
	return (0);
}

bool    part_loop_shell(t_data *data, char **av, char *read_cmd)
{
	t_tokens	*toks;
	int			err;

	err = 0;
    read_cmd = ft_expander(read_cmd, data);
	if (!read_cmd)
		return (false);
	toks = NULL;
	if (read_cmd[0] != '\0')
	{
		av = ft_command_split(read_cmd);
		if (!av)
			return (false);
		toks = ft_gimme_tokens(av);
		if (!toks)
			err = -1;
	}
	else
		return (true);
	if (ft_find_tok(toks, -1) && !err)
		err = ft_exec(av, data, toks);
	ft_free_av(av);
	if (err < 0)
		return (false);
	return (true);
}

void	ft_loop_minishell(t_data *data, char **av)
{
	char	*read_cmd;
	bool	exec_success;

	exec_success = 0;
	read_cmd = NULL;
	while (1)
	{
		if (!read_cmd)
 			read_cmd = readline("minishell: ");
		if (!read_cmd)
			break ;
		add_history(read_cmd);
		if (read_cmd[0] != '\0' && check_input(read_cmd) != 0)
			;
		else if (read_cmd[0] != '\0')
			exec_success = part_loop_shell(data, av, read_cmd);
		if (read_cmd)
		{
			free(read_cmd);
			read_cmd = NULL;
		}
		if (exec_success != true)
			break;
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	(void)av;
	signal(SIGINT, get_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (ac > 1)
		return (0);
	if (ft_env_init(&data, env))
		return (1);
	ft_loop_minishell(&data, NULL);
	ft_free_env(data.env);
	if (data.pwd)
		free(data.pwd);
	return (0);
}
