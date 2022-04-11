/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:28:04 by tde-nico          #+#    #+#             */
/*   Updated: 2022/03/22 13:28:04 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  debug  ##################### TO_REMOVE

void	debug(char *cmd, char **cmd_split, int quotes)
{
	int	i;

	i = -1;
	ft_printf("\ncmd: |%s|\n", cmd);
	ft_printf("number: %d\n", quotes);
	while (cmd_split[++i])
	{
		if (cmd_split[i][0] == 0)
			ft_printf("words: |\\0|\n");
		else
			ft_printf("words: |%s|\n", cmd_split[i]);
	}
}

// #####################  main  #####################

int	end_loop(t_shell *shell, int mod)
{
	if (mod == 2)
		shell->pipe = ft_strdup("");
	free(shell->mode);
	free(shell->cmd);
	free_matrix(shell->cmd_list);
	if (!shell->pipe)
		return (1);
	if (mod == 1)
		ft_putstr_fd(shell->pipe, 1);
	if (!shell->nest)
		free(shell->pipe);
	return (0);
}

void	cmds_process_exetend(t_shell *shell, int *i)
{
	if (!process_in_mode(shell, i))
	{
		if (shell->mode[shell->fix] == '&'
			|| shell->mode[shell->fix] == '^')
		{
			free(shell->pipe);
			shell->pipe = ft_strdup("");
		}
		process_cmd(&shell->cmd_list[*i], shell);
	}
	process_out_mode(shell, i);
	if (shell->mode[shell->fix] == '^'
		&& ft_strncmp(shell->exit_code, "0", 2))
		ft_putstr_fd(shell->pipe, 1);
}

void	rec_process(t_shell *shell, int *i)
{
	t_shell	fake;

	ft_printf("\n\tENTER\n");
	fake.cmd = ft_strdup(shell->cmd_list[(*i)]);
	fake.exit_code = ft_strdup(shell->exit_code);
	fake.env = ft_arrdup(shell->env);
	fake.fix = 0;
	fake.nest = shell->nest + 1;
	fake.pipe = NULL;
	if (shell->pipe != NULL)
		fake.pipe = ft_strdup(shell->pipe);
	fake.path = ft_strdup(shell->path);
	cmds_process_loop(&fake);
	if (shell->pipe != NULL)
		free(shell->pipe);
	shell->pipe = ft_strdup(fake.pipe);
	//ft_printf("pipe: |%s|%s|\n", fake.pipe, shell->pipe);
	free(shell->exit_code);
	shell->exit_code = ft_strdup(fake.exit_code);
	free_shell(&fake, 2);
	(shell->fix)++;
	//(*i)++;
	ft_printf("\n\tEXIT\n");
}

int	cmds_process_loop(t_shell *shell)
{
	int	i;

	i = -1;
	if (parse_commands(shell))
		return (end_loop(shell, 2));
	ft_printf("mode: -%s-\n", shell->mode);
	while (shell->cmd_list[++i])
	{
		ft_printf("|%s|%d|\n", shell->cmd_list[i], shell->fix);
		if (shell->mode[shell->fix] == '(')
			rec_process(shell, &i);
		else
			cmds_process_exetend(shell, &i);
		if (shell->mode[shell->fix] == '&')
		{
			if (!(shell->fix > 0 && ft_strchr("(<>AC", shell->mode[shell->fix - 1])))
				ft_putstr_fd(shell->pipe, 1);
			if (ft_strncmp(shell->exit_code, "0", 2))
				break ;
		}
		ft_printf("exit code: |%s|\n", shell->exit_code);
		ft_printf("pipe: |%s|\n", shell->pipe);
		if (!shell->pipe || (!ft_strncmp(shell->exit_code, "127", 4)
				&& shell->mode[shell->fix] != '^'))
			break ;
		if (shell->mode[shell->fix] == '^'
			&& !ft_strncmp(shell->exit_code, "0", 2))
		{
			if (!(shell->mode[shell->fix - 1]
				&& shell->mode[shell->fix - 1] == '('))
				ft_putstr_fd(shell->pipe, 1);
			while (ft_strchr("^(<>AC", shell->mode[shell->fix])
				&& shell->mode[shell->fix++])
			{
				if (shell->mode[shell->fix - 1] != '(')
					i++;
			}
			(shell->fix)--;
			if (shell->mode[shell->fix + 1] == '(')
				(shell->fix)++;
		}
		(shell->fix)++;
	}
	if (shell->fix && !shell->mode[shell->fix - 1])
		(shell->fix)--;
	i = 1;
	while (shell->mode[shell->fix - i] == '^')
		i++;
	if (i > 1 || (shell->mode[shell->fix - i]
		&& ft_strchr("(<>AC^", shell->mode[shell->fix - i])))
		return (end_loop(shell, 0));
	return (end_loop(shell, 1));
}

int	main_loop(t_shell *shell)
{
	while (1)
	{
		//signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
		ft_printf("%s", PROMPT);
		shell->cmd = get_line(0);
		//shell->cmd = readline(PROMPT);
		//if (ft_strncmp(shell->cmd, "", 1))
		//	add_history(shell->cmd);
		if (!shell->cmd)
			return (0 * write(1, "\n", 1));
		shell->pipe = NULL;
		if (cmds_process_loop(shell))
			break ;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_env(envp, &shell.env);
	shell.nest = 0;
	shell.exit_code = ft_strdup("0");
	shell.cmd = NULL;
	shell.path = get_path(shell.env);
	//signal(SIGINT, handle_sigint);
	//signal(SIGQUIT, handle_sigquit);
	main_loop(&shell);
	free_shell(&shell, 0);
	ft_printf("\33[0;33mlogout\33[0m\n");
	return (0);
}
