/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:51:51 by asene             #+#    #+#             */
/*   Updated: 2025/01/22 17:06:55 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>	

volatile sig_atomic_t	g_nal = 0;

void	update_shell_lvl(t_vars *vars)
{
	char	*shell_lvl;
	int		shell_lvl_value;

	shell_lvl = getenv_value(vars, "SHLVL");
	if (shell_lvl)
		shell_lvl_value = ft_atoi(shell_lvl) + 1;
	else
		shell_lvl_value = 1;
	free(shell_lvl);
	shell_lvl = ft_itoa(shell_lvl_value);
	set_env(vars, "SHLVL", shell_lvl);
	free(shell_lvl);
}

void	init_shell(t_vars *vars, char **env)
{
	vars->token_list = NULL;
	vars->exec_data = NULL;
	vars->exit_code = 0;
	setup_signal(vars);
	parse_env(env, vars);
	update_shell_lvl(vars);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_vars	vars;

	if (argc > 1)
		return (ft_fprintf(2, "Usage: %s\n", argv[0]), 1);
	init_shell(&vars, env);
	while (1)
	{
		if (g_nal == SIGINT)
			ft_putchar_fd('\n', 1);
		if (g_nal == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 1);
		g_nal = 0;
		if (readline_prompt(&vars, &input) == NULL)
			return (free(input), clean_exit(&vars, 0), 0);
		vars.token_list = tokenize(input);
		// if (!check(vars.token_list))
		// 	vars.exit_code = 2;
		// else
			vars.exit_code = execute(&vars);
		clear_token_list(&(vars.token_list));
		free(input);
	}
	return (0);
}
