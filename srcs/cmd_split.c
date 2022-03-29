/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 08:35:46 by tde-nico          #+#    #+#             */
/*   Updated: 2022/03/29 17:11:03 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  cmd words parse  #####################

int	count_spaces(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
		if (!(cmd[i] == ' '))
			break ;
	return (i);
}

int	count_word_len(char *cmd)
{
	int	i;
	int	quotes;
	int	double_quotes;

	i = -1;
	quotes = 0;
	double_quotes = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' && !double_quotes)
			quotes = (quotes + 1) % 2;
		else if (cmd[i] == '"' && !quotes)
			double_quotes = (double_quotes + 1) % 2;
		if (!quotes && !double_quotes && (cmd[i] == ' '))
			break ;
	}
	return (i);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd, "env", 4))
		return (1);
	if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd, "export", 7))
		return (1);
	if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	return (0);
}

char	**split_cmd(char *cmd, int quotes, char *pipe_in)
{
	char	**pieces;
	int		i;
	int		j;
	int		word_len;

	pieces = malloc(sizeof(char *) * (quotes + 1 + !(!pipe_in)));
	if (!pieces)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < quotes)
	{
		j += count_spaces(&cmd[j]);
		word_len = count_word_len(&cmd[j]);
		if ((cmd[j] == '"' && cmd[j + word_len - 1] == '"')
			|| (cmd[j] == '\'' && cmd[j + word_len - 1] == '\''))
			pieces[i] = ft_strndup(&cmd[j + 1], word_len - 1);
		else
			pieces[i] = ft_strndup(&cmd[j], word_len + 1);
		j += word_len;
	}
	if (pipe_in && !is_builtin(pieces[0]))
		pieces[i++] = ft_strdup(pipe_in);
	pieces[i] = 0;
	return (pieces);
}
