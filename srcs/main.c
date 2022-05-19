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

// #####################  main  #####################

int	main_loop(t_shell *shell)
{
	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
		shell->cmd = readline(PROMPT);
		if (!shell->cmd)
			return (0 * write(1, "\n", 1));
		if (ft_strncmp(shell->cmd, "", 1))
			add_history(shell->cmd);
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
	main_loop(&shell);
	free_shell(&shell, 0);
	ft_printf("\33[0;33mlogout\33[0m\n");
	return (0);
}
