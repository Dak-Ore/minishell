/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 23:34:33 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/16 16:33:01 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dest_for_cd(t_vars *vars, t_exec_data data, char **dest)
{
	if (data.argc == 1 || ft_strcmp(data.args[1], "~") == 0)
	{
		*dest = getenv_value(vars, "HOME");
		if (*dest == NULL)
			return (ft_fprintf(2, "cd: HOME not set\n"), 1);
	}
	else if (ft_strcmp(data.args[1], "-") == 0)
	{
		*dest = getenv_value(vars, "OLDPWD");
		if (*dest == NULL)
			return (ft_fprintf(2, "cd: OLDPWD not set\n"), 1);
	}
	else
		*dest = ft_strdup(data.args[1]);
	return (0);
}

int	ft_cd(t_vars *vars, t_exec_data data)
{
	char		*dest;
	struct stat	st;

	if (data.argc > 2)
		return (ft_fprintf(2, "cd: too many arguments\n"), 1);
	if (dest_for_cd(vars, data, &dest) == 1)
		return (FAILURE);
	if (stat(dest, &st) != 0)
		return (ft_fprintf(2, "cd: %s: No such file or directory\n", dest),
			free(dest), FAILURE);
	if (S_ISDIR(st.st_mode))
	{
		set_env(vars, "OLDPWD", getcwd(NULL, 0));
		chdir(dest);
		set_env(vars, "PWD", getcwd(NULL, 0));
		free(dest);
	}
	else
		return (ft_fprintf(2, "cd: %s: not a directory\n", dest),
			free(dest), FAILURE);
	return (SUCCESS);
}

int	ft_echo(t_vars *vars, t_exec_data data)
{
	int		i;
	int		new_line;

	(void)vars;
	i = 1;
	new_line = 1;
	while (i < data.argc)
	{
		if (ft_strcmp(data.args[i], "-n"))
			break ;
		new_line = 0;
		i++;
	}
	while (i < data.argc)
	{
		ft_putstr_fd(data.args[i++], 1);
		if (i < data.argc)
			ft_putchar_fd(' ', 1);
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
	return (SUCCESS);
}

int	ft_exit(t_vars *vars, t_exec_data data)
{
	int	exit_code;

	exit_code = vars->exit_code;
	if (data.argc == 2)
	{
		exit_code = ft_atoi(data.args[1]); //TODO atol
		if (exit_code > MAX_EXIT_CODE)
			exit_code = MAX_EXIT_CODE;
	}
	else if (data.argc > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		exit_code = FAILURE;
	}
	free_split(data.args);
	clean_exit(vars, exit_code);
	return (exit_code);
}

int	ft_pwd(t_vars *vars, t_exec_data data)
{
	char	*pwd;

	(void)vars;
	(void)data;
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	return (SUCCESS);
}
