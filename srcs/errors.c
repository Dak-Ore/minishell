/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:45:51 by asene             #+#    #+#             */
/*   Updated: 2025/01/22 21:48:15 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	file_error(char *file)
{
	char *msg;

	if (access(file, F_OK) == 0)
		msg = "Permission denied";
	else
		msg = "No such file or directory";
	ft_fprintf(2, "minishell: %s: %s\n", file, msg);
}

void	syntaxe_error(t_token *token)
{
	ft_fprintf(2, "minishell: syntax error near unexpected token `%s'\n",
		token_str(token));
}
