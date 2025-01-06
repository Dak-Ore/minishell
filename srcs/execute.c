/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asene <asene@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:56:53 by asene             #+#    #+#             */
/*   Updated: 2025/01/06 16:02:44 by asene            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	end_exec(t_exec_data data, int pid)
{
	int		status;
	pid_t	wpid;

	if (data.path)
		free(data.path);
	if (data.args)
		free_split(data.args);
	wpid = waitpid(pid, &status, 0);
	g_nal = 0;
	if (wpid == -1)
		return (wpid);
	if (!WIFEXITED(status))
		return (-2);
	return (WEXITSTATUS(status));
}

pid_t	exec_cmd(t_vars *vars, t_exec_data data)
{
	pid_t		pid;

	(void)vars;
	if (ft_strcmp("./minishell", data.path) == 0)
		g_nal = 2;
	pid = fork();
	if (pid == -1)
		perror("Error on fork ");
	else if (pid == 0)
	{
		if (dup2(data.fd_in, 0) != 0)
			close(data.fd_in);
		if (dup2(data.fd_out, 1) != 1)
			close(data.fd_out);
		execve(data.path, data.args, vars->env);
	}
	return (pid);
}

void	execute(t_vars *vars)
{
	t_word_type	type;
	pid_t		pid;
	t_exec_data	data;

	vars->current_token = vars->token_list;
	while (vars->current_token->token.type != TOKEN_END)
	{
		data = build_exec(vars);
		type = cmd_or_file(data.args[0], vars->env);
		if (type == W_BUILTIN)
			exec_builtin(vars, data);
		else if (type == W_CMD || type == W_EXECUTABLE)
		{
			g_nal = 1;
			pid = exec_cmd(vars, data);
			end_exec(data, pid);
		}
		else if (data.args[0])
			ft_fprintf(2, "%s: command not found\n", data.args[0]);
		if (vars->current_token->token.type == TOKEN_PIPE)
			vars->current_token = vars->current_token->next;
	}
}
