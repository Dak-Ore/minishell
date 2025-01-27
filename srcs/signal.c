/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 09:30:16 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/27 15:50:12 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sign)
{
	if (sign == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_nal = 0;
	}
}

void	catch_sign(int sign)
{
	g_nal = sign;
}

void	handle_heredoc(int sig)
{
	g_nal = sig;
	if (sig == SIGINT)
		rl_done = 1;
}

int	start_signal(t_vars *vars)
{
	vars->sa.sa_handler = &handle_signal;
	if (sigaction(SIGINT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	if (sigaction(SIGPIPE, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	vars->sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}

int	stop_signal(t_vars *vars)
{
	vars->sa.sa_handler = &catch_sign;
	if (sigaction(SIGINT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	if (sigaction(SIGQUIT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}
