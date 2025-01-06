/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:45:46 by asene             #+#    #+#             */
/*   Updated: 2025/01/06 15:51:58 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*replace_vars(char *str, char **env)
{
	int		i;
	t_list	*lst;
	char	*var;
	char	**array;
	char	*res;

	lst = NULL;
	while (*str)
	{
		i = 0;
		while (str[i] && str[i] != '$')
			i++;
		ft_lstadd_back(&lst, ft_lstnew(ft_substr(str, 0, i)));
		str = str + i;
		if (*str == '$' && ++str)
		{
			var = grab_word(&str);
			ft_lstadd_back(&lst, ft_lstnew(getenv_value(env, var)));
			str += ft_strlen(var);
			free(var);
		}
	}
	array = (char **)list_to_array(lst);
	res = ft_strnjoin(array, count_line(array), "");
	return (ft_lstclear(&lst, NULL), free(array), res);
}

char	*eval_string(char *str, char **env)
{
	char	*p;
	char	*res;

	p = NULL;
	if (ft_strchr("'\"", *str))
	{
		if (str[ft_strlen(str) - 1] != str[0])
			return (NULL);
		p = ft_strdup(str + 1);
		str = p;
		str[ft_strlen(str) - 1] = '\0';
	}
	res = replace_vars(str, env);
	if (p)
		free (p);
	return (res);
}

char	*build_word(t_vars *vars, t_tokenlist **lst)
{
	char	*value;
	char	*tmp;

	value = ft_calloc(1, sizeof(char));
	while ((*lst)->token.type == TOKEN_WORD)
	{
		tmp = eval_string((*lst)->token.value, vars->env);
		str_append(&value, tmp);
		free(tmp);
		(*lst) = (*lst)->next;
	}
	return (value);
}

t_exec_data	build_exec(t_vars *vars)
{
	t_exec_data	data;
	t_list		*lst;
	t_tokenlist	**t;

	t = &vars->current_token;
	lst = NULL;
	data.fd_in = 0;
	data.fd_out = 1;
	while ((*t) && (*t)->token.type != TOKEN_PIPE && (*t)->token.type != TOKEN_END)
	{
		if ((*t)->token.type == TOKEN_WORD)
			ft_lstadd_back(&lst, ft_lstnew(build_word(vars, t)));
		else
			(*t) = (*t)->next;
	}
	data.args = (char **)list_to_array(lst);
	ft_lstclear(&lst, NULL);
	data.argc = count_line(data.args);
	data.path = NULL;
	if (1)
		data.path = search_path(vars->env, data.args[0]);
	return (data);
}
