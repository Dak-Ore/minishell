/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:34:03 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/24 13:56:06 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	signal_heredoc(t_vars *vars)
{
	vars->sa.sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	vars->sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &vars->sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (1);
}

int		heredoc_delimiter(t_vars *vars, char *delimiter, char **new_delimiter)
{
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
	{
		*new_delimiter = eval_string(vars, delimiter);
		return (1);
	}
	*new_delimiter = delimiter;
	return (0);
}

void	heredoc_child(char *delimiter, char *name, t_vars *vars)
{
	char	*line;
	int		fd;
	char	*new_delimiter;
	int		has_quote;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(name);
	has_quote = heredoc_delimiter(vars, delimiter, &new_delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, new_delimiter) == 0 || g_nal == SIGINT)
		{
			free(line);
			break ;
		}
		if (has_quote)
			ftf_print_var(fd, line, vars);
		else
			ft_fprintf(fd, "%s\n", line);
		free(line);
	}
	free(new_delimiter);
	close(fd);
	clean_exit(vars, 0);
}

void	heredoc_killer(int nbheredoc)
{
	int		i;
	char	*name;
	char	*number;

	if (nbheredoc == 0)
		return ;
	i = 0;
	while (i <= nbheredoc)
	{
		number = ft_itoa(i);
		name = ft_strjoin(".heredoc", number);
		unlink(name);
		i++;
		free(name);
		free(number);
	}
}

int	heredoc(char *delimiter, t_vars *vars)
{
	int		fd;
	pid_t	pid;
	char	*name;
	char	*number;

	number = ft_itoa(vars->nbheredoc);
	name = ft_strjoin(".heredoc", number);
	free(number);
	pid = fork();
	if (pid == 0 && signal_heredoc(vars))
		heredoc_child(delimiter, name, vars);
	else
	{
		waitpid(pid, 0, 0);
		if (g_nal == SIGINT)
		{
			fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(fd);
		}
	}
	fd = open(name, O_RDONLY, 0644);
	free(name);
	vars->nbheredoc = vars->nbheredoc + 1;
	return (fd);
}
