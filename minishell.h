/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:52:55 by asene             #+#    #+#             */
/*   Updated: 2025/01/03 13:53:26 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>

extern int	g_nal;

typedef enum e_word_type
{
	W_NONE,
	W_BUILTIN,
	W_EXECUTABLE,
	W_FILE,
	W_CMD
}	t_word_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SPACE,
	TOKEN_VAR,
	TOKEN_END
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

typedef struct s_tokenlist
{
	t_token				token;
	struct s_tokenlist	*next;
}	t_tokenlist;

typedef struct s_exec_data
{
	char	*path;
	char	**args;
	int		fd_in;
	int		fd_out;
	int		pipe[0];
}	t_exec_data;

typedef struct s_vars
{
	char		**env;
	char		**builtins;
	int			envsize;
	char		*prompt;
	t_tokenlist	*token_list;
	t_tokenlist	*current_token;
}	t_vars;

char		*ft_strdoublejoin(char const *s1, char const *s2, char const *s3);
void		**list_to_array(t_list *lst);

t_tokenlist	*tokenize(const char *input);
void		token_append(t_tokenlist **lst, t_token_type type, char *value);
void		clear_token_list(t_tokenlist **t);

char		*search_path(char **env, char *cmd);
char		*getenv_value(char **env, char *word);
t_word_type	cmd_or_file(char *token, char **env);
int			setup_signal(void);

void		exec_builtin(t_vars *vars);
int			count_line(char **str);
void		parse_env(char **env, t_vars *vars);
void		execute(t_vars *vars);
int			correct_path(char **env, char *cmd);

#endif