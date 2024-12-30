/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:09:56 by rsebasti          #+#    #+#             */
/*   Updated: 2024/12/30 16:16:27 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_env(char **env, t_vars *vars)
{
	char	**new_env;
	int		i;

	vars->envsize = count_line(env);
	new_env = malloc(sizeof(char *) * (vars->envsize + 13));
	if (!new_env)
		return ;
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	vars->env = new_env;
}
