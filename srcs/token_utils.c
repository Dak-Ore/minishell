/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:50:48 by asene             #+#    #+#             */
/*   Updated: 2025/01/22 21:50:51 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection(t_token t)
{
	if (t.type == TOKEN_REDIRECT_IN
		|| t.type == TOKEN_HEREDOC
		|| t.type == TOKEN_REDIRECT_OUT
		|| t.type == TOKEN_APPEND)
		return (true);
	return (false);
}

bool	is_limit_token(t_token t)
{
	if (t.type == TOKEN_PIPE
		|| t.type == TOKEN_END)
		return (true);
	return (false);
}

char	*token_str(t_token *t)
{
	if (!t)
		return ("newline");
	if (t->type == TOKEN_WORD)
		return (t->value);
	else if (t->type == TOKEN_PIPE)
		return ("|");
	else if (t->type == TOKEN_REDIRECT_IN)
		return ("<");
	else if (t->type == TOKEN_HEREDOC)
		return ("<<");
	else if (t->type == TOKEN_REDIRECT_OUT)
		return (">");
	else if (t->type == TOKEN_APPEND)
		return (">>");
	return (NULL);
}