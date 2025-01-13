/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 21:36:48 by asene             #+#    #+#             */
/*   Updated: 2025/01/09 16:45:16 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export") \
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "echo") \
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "env") \
		|| !ft_strcmp(cmd, "unset"))
		return (1);
	else
		return (0);
}

void	(*get_builtin(char *cmd))(t_vars *vars, t_exec_data data)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd);
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit);
	else if (ft_strcmp(cmd, "env") == 0)
		return (ft_env);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset);
	return (NULL);
}

void	exec_builtin(t_vars *vars, t_exec_data data)
{
	void	(*builtin)(t_vars *, t_exec_data);
	pid_t	pid;
	int		is_fork;

	is_fork = !isatty(data.fd_in) || !isatty(data.fd_out);
	if (is_fork)
	{
		pid = fork();
		if (pid == -1)
			return (perror("Error on fork "));
		else if (pid > 0)
			return ;
		if (dup2(data.fd_in, 0) != 0)
			close(data.fd_in);
		if (dup2(data.fd_out, 1) != 1)
			close(data.fd_out);
	}
	builtin = get_builtin(data.args[0]);
	if (!builtin)
		return ;//(EXIT_FAILURE);
	if (is_fork)
		return ((builtin(vars, data))); //exit
	else
		return (builtin(vars, data));
	return ;
}
