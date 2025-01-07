/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 09:30:16 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/07 15:59:24 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sign, siginfo_t *info, void *context)
{
	(void) context;
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
	{
		if (g_nal == 1)
		{
			kill(info->si_pid, SIGQUIT);
			g_nal = 0;
			ft_fprintf(2, "Quit (core dumped)\n");
		}
	}

}

int	setup_signal(void)
{
	struct sigaction sa;

	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
	return (0);
}
