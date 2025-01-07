/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 23:34:33 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/07 21:04:08 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_vars *vars, t_exec_data data)
{
	struct stat	st;

	if (data.argc != 2)
		return ;
	if (stat(data.args[1], &st) != 0)
		return (perror("stat: "), (void)0);
	if (S_ISDIR(st.st_mode))
	{
		chdir(data.args[1]);
		set_env(vars, "OLDPWD", getenv_value(vars, "PWD"));
		set_env(vars, "PWD", getcwd(NULL, 0));
	}
	else
		return ;
}

void	ft_export(t_vars *vars, t_exec_data data)
{
	int		i;
	char	**key_value;

	if (data.argc == 1)
		return (ft_env(vars, data));
	i = 1;
	while (i < data.argc)
	{
		key_value = ft_split(data.args[i], '=');
		if (count_line(key_value) != 2)
			ft_fprintf(2, "export: '%s': not a valid identifier\n",
				data.args[i]);
		else
			set_env(vars, key_value[0], key_value[1]);
		free_split(key_value);
		i++;
	}
}

void	ft_pwd(t_vars *vars, t_exec_data data)
{
	if (data.argc > 1)
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		return ;
	}
	printf("%s\n", getenv_value(vars, "PWD"));
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

	exit_code = 0;
	if (data.argc == 2)
		exit_code = ft_atoi(data.args[1]);
	else if (data.argc > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		return ;
	}
	ft_lstclear(&vars->env, (void (*)(void *))free_split);
	free(vars->prompt);
	clear_token_list(&(vars->token_list));
	free_split(data.args);
	rl_clear_history();
	exit(exit_code);
}

void	ft_env(t_vars *vars, t_exec_data data)
{
	t_list	*env;
	char	**value;

	(void)data;
	env = vars->env;
	while (env)
	{
		value = env->content;
		ft_printf("%s=%s\n", value[0], value[1]);
		env = env->next;
	}
}

void	ft_unset(t_vars *vars, t_exec_data data)
{
	int		i;

	i = 1;
	while (i < data.argc)
		unset_env(vars, data.args[i++]);
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
