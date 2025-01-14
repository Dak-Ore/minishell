/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 21:34:32 by asene             #+#    #+#             */
/*   Updated: 2025/01/14 11:46:00 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_vars *vars, t_exec_data data)
{
	int		i;
	char	**key_value;

	if (data.argc == 1)
		return (ft_env(vars, data));
	i = 1;
	while (i < data.argc)
	{
		key_value = ft_split(data.args[i], '=');
		if (count_line(key_value) != 2)
			ft_fprintf(2, "export: '%s': not a valid identifier\n",
				data.args[i]);
		else
			set_env(vars, key_value[0], key_value[1]);
		free_split(key_value);
		i++;
	}
	return (0);
}

int	ft_env(t_vars *vars, t_exec_data data)
{
	t_list	*env;
	char	**value;

	(void)data;
	if (data.argc > 1)
		return (ft_fprintf(2, "env: too many arguments\n"), 1);
	env = vars->env;
	while (env)
	{
		value = env->content;
		ft_printf("%s=%s\n", value[0], value[1]);
		env = env->next;
	}
	return (0);
}

int	ft_unset(t_vars *vars, t_exec_data data)
{
	int		i;

	i = 1;
	while (i < data.argc)
		unset_env(vars, data.args[i++]);
	return (0);
}
