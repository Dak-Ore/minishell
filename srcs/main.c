/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:51:51 by asene             #+#    #+#             */
/*   Updated: 2025/01/23 00:34:50 by asene            ###   ########.fr       */
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
	vars->sa.sa_flags = SA_RESTART;
	sigemptyset(&vars->sa.sa_mask);
	start_signal(vars);
	parse_env(env, vars);
	update_shell_lvl(vars);
}
void	sig_update(void)
{
	if (g_nal == SIGINT)
		ft_putchar_fd('\n', 1);
	if (g_nal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 1);
	g_nal = 0;
}

t_exec_mode	move_to_next(t_token **token)
{
	t_exec_mode	mode;
	t_token		*tmp;

	while (*token && !is_limit_token(**token))
	{
		tmp = *token;
		*token = (*token)->next;
		(void)tmp; // free token
	}
	if (*token)
	{
		if ((*token)->type == TOKEN_END)
			mode = REGULAR;
		else if ((*token)->type == TOKEN_AND)
			mode = AND;
		else if ((*token)->type == TOKEN_OR)
			mode = OR;
		*token = (*token)->next;
		return (mode);
	}
	return (REGULAR);
}

void	prompt_loop(t_vars *vars)
{
	char		*input;
	t_exec_mode	mode;

	mode = REGULAR;
	sig_update();
	if (readline_prompt(vars, &input) == NULL)
		return (free(input), clean_exit(vars, 0));
	vars->token_list = tokenize(input);
	free(input);
	if (!check(vars->token_list))
		vars->exit_code = 2;
	else
	{
		while (vars->token_list)
		{
			if (mode == REGULAR
				|| (mode == AND && vars->exit_code == 0)
				|| (mode == OR && vars->exit_code != 0))
				vars->exit_code = execute(vars);
			mode = move_to_next(&vars->token_list);
		}
		clear_token_list(&vars->token_list);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;

	if (argc > 1)
		return (ft_fprintf(2, "Usage: %s\n", argv[0]), 1);
	init_shell(&vars, env);
	while (1)
		prompt_loop(&vars);
	return (0);
}
