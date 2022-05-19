/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:48:27 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/13 10:50:26 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  debug  ##################### TO_REMOVE

void	debug_cmd(char *cmd, char **cmd_split)
{
	int	i;

	i = -1;
	ft_printf("\ncmd: |%s|\n", cmd);
	while (cmd_split[++i])
	{
		if (cmd_split[i][0] == 0)
			ft_printf("words: |\\0|\n");
		else
			ft_printf("words: |%s|\n", cmd_split[i]);
	}
}

int	debug_pipe(t_shell *shell, int verbose)
{
	if (ft_strnstr(shell->cmd, "|", ft_strlen(shell->cmd)))
	{
		free(shell->exit_code);
		replace_env(&shell->cmd, shell);
		shell->exit_code = ft_itoa(system(shell->cmd));
		if (verbose)
		{
			debug_cmd(shell->cmd, shell->env);
			ft_printf("\nexit_code: |%s|\n", shell->exit_code);
			ft_printf("\ncmd: |%s|\n", shell->mode);
		}
		ft_printf("\nexit_code: |%s|\n", shell->exit_code);
		shell->pipe = ft_strdup("");
		return (1);
	}
	return (0);
}
