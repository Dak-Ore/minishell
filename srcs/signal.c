/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 09:30:16 by rsebasti          #+#    #+#             */
/*   Updated: 2024/12/29 14:17:30 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sign)
{
	if (sign == SIGINT)
	{
		if (g_nal != 2)
			ft_printf("\n");
		if (g_nal == 0)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		if (g_nal == 1)
			g_nal = 0;
	}
	if (sign == SIGQUIT)
		ft_printf("SIQUIT RECU \n");
}

int	setup_sign(void)
{
	if (signal(SIGINT, handle_signal) == SIG_ERR)
		return (perror("Erreur lors de la configuration de SIGINT"), 1);
	if (signal(SIGQUIT, handle_signal) == SIG_ERR)
		return (perror("Erreur lors de la configuration de SIGQUIT"), 1);
	return (0);
}
