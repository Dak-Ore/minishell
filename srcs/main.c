/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsebasti <rsebasti@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:51:51 by asene             #+#    #+#             */
/*   Updated: 2025/01/07 13:51:56 by rsebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

volatile int	g_nal = 0;

void	analyze_token(t_token *token, char **env)
{
	if (cmd_or_file(token->value, env) == W_CMD)
		printf("'%s' est une commande valide.\n", token->value);
	else if (cmd_or_file(token->value, env) == W_FILE)
		printf("'%s' est un fichier existant.\n", token->value);
	else if (cmd_or_file(token->value, env) == W_EXECUTABLE)
		printf("'%s' est un fichier executable.\n", token->value);
	else if (cmd_or_file(token->value, env) == W_BUILTIN)
		printf("'%s' est une commande builtin.\n", token->value);
	else
		printf("'%s' est une commande invalide.\n", token->value);
}

void	print_tokens(t_tokenlist *list, char **env)
{
	while (list)
	{
		if (list->token.type == TOKEN_WORD)
			analyze_token(&list->token, env);
		else
			ft_printf("Type: %d, %s\n", list->token.type, list->token.value);
		list = list->next;
	}
}

void	init_shell(t_vars *vars, char **env)
{
	vars->token_list = NULL;
	vars->current_token = NULL;
	setup_signal();
	parse_env(env, vars);
}

char	*set_prompt(t_vars *vars)
{
	int		i;
	char	*prompt[5];
	char	*pwd;
	char	*home;

	i = 0;
	prompt[i++] = "minishell:";
	pwd = getcwd(NULL, 150);
	home = getenv_value(vars->env, "HOME");
	if (pwd && home && ft_strncmp(home, pwd, ft_strlen(home)) == 0)
	{
		prompt[i++] = "~";
		prompt[i++] = pwd + ft_strlen(home);
	}
	else if (pwd)
		prompt[i++] = pwd;
	prompt[i++] = " > ";
	vars->prompt = ft_strnjoin(prompt, i, "");
	return (free(pwd), vars->prompt);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_vars	vars;

	if (argc > 1)
		return (ft_fprintf(2, "Usage: %s\n", argv[0]), 1);
	init_shell(&vars, env);
	while (1)
	{
		input = readline(set_prompt(&vars));
		add_history(input);
		if (input == NULL)
			return (free(input), free_split(vars.env), free(vars.prompt), 0);
		vars.token_list = tokenize(input);
		execute(&vars);
		clear_token_list(&(vars.token_list));
		free(input);
		free(vars.prompt);
	}
	return (0);
}
