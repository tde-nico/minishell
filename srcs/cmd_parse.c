/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:48:27 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/02 10:09:18 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  cmd parse  #####################

int	count_cmd(char *cmd, int count, int quotes, int double_quotes)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' && !double_quotes)
			quotes = (quotes + 1) % 2;
		else if (cmd[i] == '"' && !quotes)
			double_quotes = (double_quotes + 1) % 2;
		else if (ft_strchr("|<>", cmd[i]) && !quotes && !double_quotes)
		{
			if (++count && ft_strchr("<>|", cmd[i + 1]))
				i++;
		}
		else if (cmd[i] == '&' && cmd[i + 1] == '&'
			&& !quotes && !double_quotes)
		{
			count++;
			i++;
		}
	}
	return (count);
}

void	parse_modes(t_shell *shell, int **q)
{
	shell->mode = ft_charjoin(shell->mode, shell->cmd[(*q)[3]]);
	if (shell->cmd[(*q)[3]] == '>'
		&& shell->cmd[(*q)[3] + 1] == '>')
		shell->mode[(*q)[2]] += 5 + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '<'
		&& shell->cmd[(*q)[3] + 1] == '<')
		shell->mode[(*q)[2]] += 5 + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '|'
		&& shell->cmd[(*q)[3] + 1] == '|')
		shell->mode[(*q)[2]] = '^' + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '&'
		&& shell->cmd[(*q)[3] + 1] == '&')
		shell->mode[(*q)[2]] = '&' + (0 * (*q)[3]++);
}

void	parse_commands_loop(t_shell *shell, int **q)
{
	while (++(*q)[2] < (*q)[4])
	{
		(*q)[3]++;
		shell->cmd_list[(*q)[2]] = malloc(sizeof(char) * 1);
		shell->cmd_list[(*q)[2]][0] = 0;
		while (shell->cmd[(*q)[3]])
		{
			if (shell->cmd[(*q)[3]] == '\'' && !(*q)[1])
				(*q)[0] = ((*q)[0] + 1) % 2;
			else if (shell->cmd[(*q)[3]] == '"' && !(*q)[0])
				(*q)[1] = ((*q)[1] + 1) % 2;
			else if (!(*q)[0] && !(*q)[1]
				&& ft_strchr("|<>&", shell->cmd[(*q)[3]]))
			{
				parse_modes(shell, q);
				break ;
			}
			shell->cmd_list[(*q)[2]] = ft_charjoin(shell->cmd_list[(*q)[2]],
					shell->cmd[(*q)[3]++]);
		}
	}
}

void	parse_commands(t_shell *shell)
{
	int	*qsijc;

	qsijc = malloc(sizeof(int) * 6);
	shell->mode = malloc(sizeof(char) * 1);
	if (!qsijc || !shell->mode)
		return ;
	qsijc[0] = 0;
	qsijc[1] = 0;
	qsijc[2] = -1;
	qsijc[3] = -1;
	qsijc[4] = count_cmd(shell->cmd, 1, 0, 0);
	shell->mode[0] = '\0';
	shell->fix = 0;
	shell->cmd_list = NULL;
	shell->cmd_list = malloc(sizeof(char *) * (qsijc[4] + 1));
	if (!shell->cmd_list)
		return ;
	parse_commands_loop(shell, &qsijc);
	shell->cmd_list[qsijc[2]] = 0;
	free(qsijc);
}
