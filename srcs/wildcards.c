/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 10:11:34 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/13 11:46:13 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	quotes_check(t_shell *shell)
{
	char	*ptr;
	int		i;
	int		qs[2];

	i = -1;
	ptr = &shell->cmd[0];
	while (++i <= shell->j)
	{
		ft_printf("%s\n", ptr);
		ptr += (count_char(ptr, '\'') + count_char(ptr, '"'));
		ft_printf("%s\n", ptr);
		ptr += count_char(ptr, ' ');
		ft_printf("%s\n", ptr);
		ptr += (count_char(ptr, '\'') + count_char(ptr, '"'));
		ft_printf("%s\n", ptr);
		ptr += ft_strlen(shell->words[i]);
		ft_printf("%s\n", ptr);
		//ptr += (count_char(ptr, '\'') + count_char(ptr, '"'));
		//ft_printf("%s\n", ptr);
	}
	//ft_strnstr(shell->cmd, shell->words[shell->j], ft_strlen(shell->cmd) + 1);
	if (!ptr)
		return (0);
	qs[0] = 0;
	qs[1] = 0;
	i = 0;
	while ((ptr - shell->cmd - i) > 0)
	{
		ft_printf("%d %d %d\n", (ptr - shell->cmd - i), qs[0], qs[1]);
		if ((shell->cmd)[i] == '\'' && !qs[1])
			qs[0] = (qs[0] + 1) % 2;
		else if ((shell->cmd)[i] == '"' && !qs[0])
			qs[1] = (qs[1] + 1) % 2;
		i++;
	}
	if (!qs[0] && !qs[1])
		return (1);
	return (0);
}

void	wild_loop(char **cmd, t_shell *shell, char **tmp, char **new)
{
	while (shell->words[shell->j][++(shell->i)])
	{
		if (shell->words[shell->j][shell->i] == '*' && quotes_check(shell))
		{
			parse_wild(new, &shell->words[shell->j]);
			break ;
		}
		else
			*tmp = ft_charjoin(*tmp, shell->words[shell->j][shell->i]);
	}
	if (shell->j)
		*cmd = ft_charjoin(*cmd, '\'');
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
	if (shell->j)
		*cmd = ft_charjoin(*cmd, '\'');
}

void	replace_wild(char **cmd, t_shell *shell)
{
	char	*tmp;
	char	*new;

	shell->words = split_cmd(*cmd, count_quotes(*cmd), NULL);
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
