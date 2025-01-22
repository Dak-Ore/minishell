/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 11:26:31 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/22 21:50:57 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*next_token(t_token *tok_list)
{
	tok_list = tok_list->next;
	while (tok_list)
	{
		if (tok_list->type != TOKEN_SPACE)
			return (tok_list);
		tok_list = tok_list->next;
	}
	return (tok_list);
}

int	check(t_token *tok_list)
{
	t_token	*next;

	if (tok_list->type == TOKEN_PIPE)
		return (syntaxe_error(tok_list), 0);
	while (tok_list)
	{
		next = next_token(tok_list);
		if ((is_redirection(*tok_list) && (!next || next->type != TOKEN_WORD))
			|| (tok_list->type == TOKEN_PIPE && (!next || next->type == TOKEN_PIPE)))
			return (syntaxe_error(next), 0);
		tok_list = next;
	}
	return (1);
}
