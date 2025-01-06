/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:52:55 by asene             #+#    #+#             */
/*   Updated: 2025/01/06 13:42:55 by rsebasti         ###   ########.fr       */
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
	int		argc;
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
int			count_line(char **str);
void		**list_to_array(t_list *lst);
int			skip_spaces(char **str);
char		*str_append(char **dest, char *next);

t_tokenlist	*tokenize(char *input);
char		*grab_word(char **p);
void		token_append(t_tokenlist **lst, t_token_type type, char *value);
void		clear_token_list(t_tokenlist **t);

char		*search_path(char **env, char *cmd);
char		*getenv_value(char **env, char *word);
t_word_type	cmd_or_file(char *token, char **env);
int			setup_signal(void);

t_exec_data	build_exec(t_vars *vars);

void		exec_builtin(t_vars *vars, t_exec_data data);
void		parse_env(char **env, t_vars *vars);
void		execute(t_vars *vars);
int			correct_path(char **env, char *cmd);
int			syntax_check(t_vars *vars);

void		ft_cd(t_vars *vars, t_exec_data data);
void		ft_export(t_vars *vars, t_exec_data data);
void		ft_pwd(t_vars *vars, t_exec_data data);
void		ft_echo(t_vars *vars, t_exec_data data);
void		ft_exit(t_vars *vars, t_exec_data data);
void		ft_env(t_vars *vars, t_exec_data data);
void		ft_unset(t_vars *vars, t_exec_data data);

#endif