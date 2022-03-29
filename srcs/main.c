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

// #####################  debug  #####################

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

int	end_loop(t_shell *shell)
{
	free(shell->mode);
	free(shell->cmd);
	free_matrix(shell->cmd_list);
	if (!shell->pipe)
		return (1);
	ft_putstr_fd(shell->pipe, 1);
	free(shell->pipe);
	return (0);
}

int	cmds_process_loop(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->cmd_list[++i])
	{
		if (!process_in_mode(shell, &i))
			process_cmd(&shell->cmd_list[i], shell);
		process_out_mode(shell, &i);
		ft_printf("exit code: |%s|\n", shell->exit_code);
		ft_printf("pipe: |%s|\n", shell->pipe);
		if (!shell->pipe || !ft_strncmp(shell->exit_code, "127", 4))
			return (1);
		(shell->fix)++;
	}
	return (0);
}

int	main_loop(t_shell *shell)
{
	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
		shell->cmd = readline(PROMPT);
		if (ft_strncmp(shell->cmd, "", 1))
			add_history(shell->cmd);
		if (!shell->cmd)
			return (0 * write(1, "\n", 1));
		parse_commands(shell);
		ft_printf("\nmode: -%s-\n", shell->mode);
		shell->pipe = NULL;
		if (cmds_process_loop(shell) || end_loop(shell))
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
	shell.exit_code = ft_strdup("0");
	shell.cmd = NULL;
	shell.path = get_path(shell.env);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	main_loop(&shell);
	free_shell(&shell, 0);
	ft_printf("\33[0;33mlogout\33[0m\n");
	return (0);
}
