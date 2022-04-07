/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:48:27 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/05 17:38:46 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  cmd parse  #####################

int	count_cmd(char *cmd, int *pc)
{
	while (cmd[++pc[3]])
	{
		if (cmd[pc[3]] == '\'' && !pc[2])
			pc[1] = (pc[1] + 1) % 2;
		else if (cmd[pc[3]] == '"' && !pc[1])
			pc[2] = (pc[2] + 1) % 2;
		else if (cmd[pc[3]] == '(' && !pc[1] && !pc[2])
			pc[4]++;
		else if (cmd[pc[3]] == ')' && !pc[1] && !pc[2])
			pc[4]--;
		else if (ft_strchr("|<>", cmd[pc[3]]) && !pc[1] && !pc[2] && !pc[4])
		{
			if (++pc[0] && ft_strchr("<>|", cmd[pc[3] + 1]))
				pc[3]++;
		}
		else if (cmd[pc[3]] == '&' && cmd[pc[3] + 1] == '&'
			&& !pc[1] && !pc[2] && !pc[4])
		{
			pc[0]++;
			pc[3]++;
		}
		if (pc[4] < 0)
			return (-1 + 0 * (ft_printf("Invalid Syntax\n")));
	}
	if (!pc[4])
		return (pc[0]);
	return (-1 + 0 * (ft_printf("Invalid Syntax\n")));
	
}

// echo a || (echo b && echo c) && echo d
// a
// d

int	parse_modes(t_shell *shell, int **q)
{
	if ((shell->cmd[(*q)[3]] == '&'
		&& shell->cmd[(*q)[3] + 1] != '&') || (*q)[5])
		return (0);
	shell->mode = ft_charjoin(shell->mode, shell->cmd[(*q)[3]]);
	//ft_printf("i: %d |%s|\n", (*q)[3], shell->mode);
	if (shell->cmd[(*q)[3]] == '>'
		&& shell->cmd[(*q)[3] + 1] == '>')
		shell->mode[ft_strlen(shell->mode) - 1] += 5 + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '<'
		&& shell->cmd[(*q)[3] + 1] == '<')
		shell->mode[ft_strlen(shell->mode) - 1] += 5 + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '|'
		&& shell->cmd[(*q)[3] + 1] == '|')
		shell->mode[ft_strlen(shell->mode) - 1] = '^' + (0 * (*q)[3]++);
	else if (shell->cmd[(*q)[3]] == '&'
		&& shell->cmd[(*q)[3] + 1] == '&')
		shell->mode[ft_strlen(shell->mode) - 1] = '&' + (0 * (*q)[3]++);
	return (1);
}

int	parse_parenthesis(t_shell *shell, int **q)
{
	if (shell->cmd[(*q)[3]] == '(')
	{
		(*q)[5]++;
		shell->mode = ft_charjoin(shell->mode, shell->cmd[(*q)[3]]);
	}
	else if (shell->cmd[(*q)[3]] == ')')
	{
		(*q)[5]--;
		if ((*q)[5])
			return (0);
		(*q)[3]++;
		return (1);
		
	}
	(*q)[3]++;
	return (0);
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
				if (parse_modes(shell, q))
					break ;
			}
			if (!(*q)[0] && !(*q)[1] && ((!(*q)[5] &&
				shell->cmd[(*q)[3]] == '(') || shell->cmd[(*q)[3]] == ')')
				&& parse_parenthesis(shell, q))
				(*q)[3] += count_char(&shell->cmd[(*q)[3] + count_char(
					&shell->cmd[(*q)[3]], ')')], ' ');// + count_char(
					//&shell->cmd[(*q)[3]], ')');
			else
				shell->cmd_list[(*q)[2]] = ft_charjoin(shell->cmd_list[(*q)[2]],
						shell->cmd[(*q)[3]++]);
		}
	}
	shell->cmd_list[(*q)[2]] = NULL;
}

void	parse_commands(t_shell *shell)
{
	int	qsijc[7];
	int	cqdip[5];
	int	*p;

	shell->mode = malloc(sizeof(char) * 1);
	qsijc[0] = 0;
	qsijc[1] = 0;
	qsijc[2] = -1;
	qsijc[3] = -1;
	cqdip[0] = 1;
	cqdip[1] = 0;
	cqdip[2] = 0;
	cqdip[3] = -1;
	cqdip[4] = 0;
	qsijc[4] = count_cmd(shell->cmd, &cqdip[0]);
	printf("par: %d\n", qsijc[4]);
	qsijc[5] = 0;
	shell->fix = 0;
	shell->cmd_list = NULL;
	shell->cmd_list = malloc(sizeof(char *) * (qsijc[4] + 1));
	if (!shell->cmd_list || !shell->mode || qsijc[4] == -1)
		return ;
	shell->mode[0] = '\0';
	p = &qsijc[0];
	parse_commands_loop(shell, &(p));
	shell->cmd_list[qsijc[2]] = 0;
}
