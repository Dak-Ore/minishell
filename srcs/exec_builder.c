/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:45:46 by asene             #+#    #+#             */
/*   Updated: 2025/01/22 21:46:48 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_exec(t_exec *data)
{
	if (data->path)
		free(data->path);
	if (data->args)
		free_split(data->args);
	if (data->pipe)
		free_exec(data->pipe);
	free(data);
}

char	*build_word(t_vars *vars, t_token **lst)
{
	char	*value;
	char	*tmp;

	value = ft_calloc(1, sizeof(char));
	while (*lst)
	{
		tmp = eval_string(vars, (*lst)->value);
		str_append(&value, tmp);
		free(tmp);
		if ((*lst)->next == NULL || ((*lst)->next && (*lst)->next->type != TOKEN_WORD))
			break ;
		(*lst) = (*lst)->next;
	}
	return (value);
}

int	get_open_flags(t_token token)
{
	int	flags;
	
	if (token.type == TOKEN_REDIRECT_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (token.type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (token.type == TOKEN_REDIRECT_IN)
		flags = O_RDONLY;
	else
		flags = 0;
	return (flags);
}

void	handle_redirect(t_vars *vars, t_exec *data, t_token **tok_lst)
{
	int		fd;
	t_token	current;

	current = **tok_lst;
	(*tok_lst) = (*tok_lst)->next;
	if (is_redirection(current) && current.type != TOKEN_HEREDOC)
		fd = open(build_word(vars, tok_lst), get_open_flags(current), 0644);
	else
		fd = heredoc((*tok_lst)->value, vars);
	if (fd == -1)
		file_error((*tok_lst)->value);
	if (current.type == TOKEN_REDIRECT_OUT || current.type == TOKEN_APPEND)
	{
		if (data->fd_out != 1)
			close(data->fd_out);
		data->fd_out = fd;
	}
	if (current.type == TOKEN_REDIRECT_IN || current.type == TOKEN_HEREDOC)
	{
		if (data->fd_in != 0)
			close(data->fd_in);
		data->fd_in = fd;
	}
}

t_exec	*build_exec(t_vars *vars, t_token *tok_lst, t_exec **data, t_exec *prev)
{
	t_list	*lst;

	lst = NULL;
	*data = ft_calloc(1, sizeof(t_exec));
	**data = (t_exec){NULL, NULL, 0, 0, 1, NULL, NULL};
	if (prev)
		(*data)->prev = prev;
	while (tok_lst && ! is_limit_token(*tok_lst))
	{
		if (tok_lst->type == TOKEN_WORD)
			ft_lstadd_back(&lst, ft_lstnew(build_word(vars, &tok_lst)));
		else if (is_redirection(*tok_lst))
			handle_redirect(vars, *data, &tok_lst);
		tok_lst = tok_lst->next;
	}
	(*data)->args = (char **)ft_lst_to_array(lst);
	ft_lstclear(&lst, NULL);
	if ((*data)->args[0])
		(*data)->path = search_path(vars, (*data)->args[0]);
	if (tok_lst && tok_lst->type == TOKEN_PIPE)
		build_exec(vars, tok_lst->next, &(*data)->pipe, *data);
	return ((*data)->argc = count_line((*data)->args), *data);
}
