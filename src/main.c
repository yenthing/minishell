/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vharkush <vharkush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 15:21:59 by vharkush          #+#    #+#             */
/*   Updated: 2023/07/28 12:56:19 by vharkush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int get_host_name(char **av, int len)
{
	int result;
	char *name;

	name = av[1];
    if (len < 1)
        return (1);
    result = printf("Host Name: %s", name);
    if (result < 0 || result >= len)
        return (1);
    return (0);
}

char *create_prompt()
{
	char *str_cwd;
	const int prompt_size = 250;

	str_cwd = malloc(prompt_size + 1);
	if (!str_cwd)
		return (NULL);
	getcwd(str_cwd, prompt_size);
	//printf("$ %s", str_cwd);
	get_host_name(&str_cwd, prompt_size);
	printf("$ %s", str_cwd);
	return (str_cwd);
}

void	ft_loop_minishell(t_data *data)
{
	(void)data;
	char *read_cmd;
	
	while(1)
	{
		read_cmd = readline(create_prompt());
		if (!read_cmd)
		printf("Unable to make prompt\n");
		continue;
	}
	if (!read_cmd)
		free(read_cmd);
}

int	main(int ac, char **av, char**env)
{
	t_data	data;

	(void)av;
	if (ac > 1)
		return (0);
	data.in = 0;
	data.out = 1;
	data.err = 1;
	if (ft_env_init(&data, env))
		return (1);
	ft_loop_minishell(&data);
	ft_free_env(data.env);
	ft_free_env(data.env_orig);
	return (0);
}
