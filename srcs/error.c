/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 11:26:31 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/06 13:43:54 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_vars *vars)
{
	t_tokenlist	*tok_list;
	const char	*tab[] = {"|", "<", ">", "<<", ">>"};

	tok_list = vars->token_list;
	if (tok_list->token.type == TOKEN_PIPE)
		return (ft_fprintf(2,
				"minishell: syntax error near unexpected token `|'"), 0);
	while (tok_list)
	{
		if (tok_list->token.type >= TOKEN_REDIRECT_IN
			&& tok_list->token.type <= TOKEN_REDIRECT_OUT
			&& ft_strlen(tok_list->token.value) == 0)
		{
			if (tok_list->next->token.type != TOKEN_END)
				return (ft_fprintf(2,
						"minishell: syntax error near unexpected token %c",
						tab[tok_list->next->token.type - 1]), 0);
			return (ft_fprintf(2,
					"minishell: syntax error near unexpected token `newline'")
				, 0);
		}
	}
	return (1);
}
