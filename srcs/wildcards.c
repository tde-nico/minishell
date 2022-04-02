/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 10:11:34 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/02 11:15:15 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wild_loop(char **cmd, t_shell *shell, char **tmp, char **new)
{
	while (shell->words[shell->j][++(shell->i)])
	{
		if (shell->words[shell->j][shell->i] == '*')
		{
			parse_wild(new, &shell->words[shell->j]);
			break ;
		}
		else
			*tmp = ft_charjoin(*tmp, shell->words[shell->j][shell->i]);
	}
	if (shell->words[shell->j][0] && shell->words[shell->j][shell->i] != '*')
	{
		shell->i = -1;
		while ((*tmp)[++(shell->i)])
			*cmd = ft_charjoin(*cmd, (*tmp)[shell->i]);
	}
	else if (shell->words[shell->j][0])
	{
		shell->i = -1;
		while ((*new)[++(shell->i)])
			*cmd = ft_charjoin(*cmd, (*new)[shell->i]);
	}
}

void	replace_wild(char **cmd, t_shell *shell)
{
	char	*tmp;
	char	*new;

	shell->words = split_cmd(*cmd, count_quotes(*cmd), shell->pipe);
	shell->j = -1;
	free(*cmd);
	*cmd = ft_strdup("");
	while (shell->words[++(shell->j)])
	{
		shell->i = -1;
		new = ft_strdup("");
		tmp = ft_strdup("");
		wild_loop(cmd, shell, &tmp, &new);
		*cmd = ft_charjoin(*cmd, ' ');
		free(tmp);
		free(new);
	}
	free_matrix(shell->words);
}
