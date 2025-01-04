/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 09:54:06 by rsebasti          #+#    #+#             */
/*   Updated: 2025/01/04 20:38:38 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **env, char *word)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], word, (int) ft_strlen(word)) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

char	*getenv_value(char **env, char *word)
{
	char	*env_str;
	int		j;

	env_str = ft_getenv(env, word);
	if (env_str == NULL)
		return (NULL);
	j = 0;
	while (env_str[j] && env_str[j] != '=')
		j++;
	return (env_str + j + 1);
}

char	*search_path(char **env, char *cmd)
{
	int		i;
	char	**split;
	char	*path;

	i = 0;
	if (access(cmd, F_OK | X_OK) == 0 && ft_strncmp(cmd, "./", 2) == 0)
		return (ft_strdup(cmd));
	split = ft_split(getenv_value(env, "PATH"), ':');
	while (split[i])
	{
		path = ft_strdoublejoin(split[i], "/", cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (free_split(split), path);
		if (path)
			free(path);
		i++;
	}
	if (split)
		free_split(split);
	return (NULL);
}

int	correct_path(char **env, char *cmd)
{
	int		i;
	char	**split;
	char	*path;

	i = 0;
	split = ft_split(getenv_value(env, "PATH"), ':');
	if (access(cmd, F_OK | X_OK) == 0 && strncmp(cmd, "./", 2) == 0)
		return (2);
	while (split[i])
	{
		path = ft_strdoublejoin(split[i], "/", cmd);
		if (access(path, F_OK | X_OK) == 0)
		{
			free(path);
			return (1);
		}
		if (path)
			free(path);
		i++;
	}
	if (split)
		free_split(split);
	return (0);
}
