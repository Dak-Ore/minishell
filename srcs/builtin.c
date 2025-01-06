/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 23:34:33 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/06 16:01:46 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_vars *vars, t_exec_data data)
{
	(void)vars;
	if (data.argc == 1);
		//HOME && set PWD VALUE
	else if (access(data.args[1], F_OK) == 0)
	{
		chdir(data.args[1]);
		// set PWD VALUE
	}
	else;
		//error directory
}

void	ft_export(t_vars *vars, t_exec_data data)
{
	if (data.argc == 1)
		return (ft_env(vars, data));
	return ;
}

void	ft_pwd(t_vars *vars, t_exec_data data)
{
	if (data.argc > 1)
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		return ;
	}
	printf("%s\n", getenv_value(vars->env, "PWD"));
}

void	ft_echo(t_vars *vars, t_exec_data data)
{
	int	i;

	(void)vars;
	i = 1;
	while (i < data.argc)
	{
		ft_putstr_fd(data.args[i], 1);
		if (++i < data.argc)
			ft_putchar_fd(' ', 1);
	}
	ft_putchar_fd('\n', 1);
	return ;
}

void	ft_exit(t_vars *vars, t_exec_data data)
{
	unsigned char	exit_code;

	(void)vars;
	exit_code = 0;
	if (data.argc == 2)
		exit_code = ft_atoi(data.args[1]);
	else if (data.argc > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		return ;
	}
	if (vars->env)
		free_split(vars->env);
	free(vars->prompt);
	clear_token_list(&(vars->token_list));
	free_split(data.args);
	rl_clear_history();
	exit(exit_code);
}

void	ft_env(t_vars *vars, t_exec_data data)
{
	int	i;

	(void)data;
	i = 0;
	while (vars->env[i])
	{
		ft_printf("%s\n",vars->env[i]);
		i++;
	}
}

void	ft_unset(t_vars *vars, t_exec_data data)
{
	(void)vars;
	(void)data;
	return ;
}

void	exec_builtin(t_vars *vars, t_exec_data data)
{
	char	*builtin;

	builtin = data.args[0];
	if (ft_strcmp(builtin, "cd") == 0)
		ft_cd(vars, data);
	else if (ft_strcmp(builtin, "export") == 0)
		ft_export(vars, data);
	else if (ft_strcmp(builtin, "pwd") == 0)
		ft_pwd(vars, data);
	else if (ft_strcmp(builtin, "echo") == 0)
		ft_echo(vars, data);
	else if (ft_strcmp(builtin, "exit") == 0)
		ft_exit(vars, data);
	else if (ft_strcmp(builtin, "env") == 0)
		ft_env(vars, data);
	else if (ft_strcmp(builtin, "unset") == 0)
		ft_unset(vars, data);
}
