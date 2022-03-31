/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   cmd_process.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: tde-nico <tde-nico@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/29 13:27:43 by tde-nico		  #+#	#+#			 */
/*   Updated: 2022/03/29 13:27:43 by tde-nico		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

// #####################  cmd processing  #####################

int	count_quotes(char *cmd)
{
	int	i;
	int	count;
	int	double_quotes;
	int	quotes;

	i = -1;
	count = 0;
	double_quotes = 0;
	quotes = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '"' && !quotes)
			double_quotes = (double_quotes + 1) % 2;
		else if (cmd[i] == '\'' && !double_quotes)
			quotes = (quotes + 1) % 2;
		if ((!quotes && !double_quotes) && !(cmd[i] == ' ')
			&& ((cmd[i + 1] == ' ') || !cmd[i + 1]))
			count++;
	}
	return (count);
}

int	process_builtins(t_shell *shell)
{
	if (!ft_strncmp(shell->words[0], "echo", 5))
		echo(shell);
	else if (!ft_strncmp(shell->words[0], "env", 4))
		env(shell);
	else if (!ft_strncmp(shell->words[0], "cd", 3))
		cd(shell);
	else if (!ft_strncmp(shell->words[0], "export", 7))
		export(shell);
	else if (!ft_strncmp(shell->words[0], "unset", 6))
		unset(shell);
	else if (!ft_strncmp(shell->words[0], "pwd", 4))
		shell->pipe = ft_charjoin(ft_strdup(shell->path), '\n');
	else if (!ft_strncmp(shell->words[0], "ls", 3))
		ls(shell);
	else
		return (0);
	return (1);
}

int	process_programs(t_shell *shell)
{
	if (!shell->words[0] || !shell->words[0][0])
		return (1);
	if (!ft_strncmp(shell->words[0], "exit", 5))
		return (0);
	if (!process_builtins(shell))
	{
		execute_pipe(shell);
		if (!ft_strncmp(shell->exit_code, "127", 4))
			ft_printf("\033[0;31m%s: command not found\033[0m\n",
				shell->words[0]);
	}
	return (0);
}

void	wild(t_shell *shell, char **new, char **cmd, int i)
{
	int		j;
	int		k;
	int		len;
	char	**tmp_split;
	char	**list;
	char	*tmp;
	int		start;
	int		end;

	list = list_dir(".");
	j = -1;
	i = 1;
	while ((*cmd)[++j])
	{
		if ((*cmd)[j] != '*')
			i = 0;
	}
	if (i)
	{
		j = -1;
		while (list[++j])
		{
			i = -1;
			while (list[j][++i])
				*new = ft_charjoin(*new, list[j][i]);
			*new = ft_charjoin(*new, ' ');
		}
		free_matrix(list);
		return ;
	}
	start = 1;
	if ((*cmd)[0] == '*')
		start = 0;
	end = 1;
	if ((*cmd)[ft_strlen((*cmd)) - 1] == '*')
		end = 0;
	tmp_split = ft_split((*cmd), '*');
	j = -1;
	while (list[++j])
	{
		k = 0;
		i = -1;
		len = ft_strlen(list[j]);
		while (tmp_split[++i])
		{
			if (!tmp_split[i + 1])
				len++;
			tmp = ft_strnstr(&(list[j][k]), tmp_split[i], len);
			if (tmp)
			{
				k += ft_strlen(tmp_split[i]) + tmp - &(list[j][k]);
				ft_printf("%c %d %d\n", list[j][k], k, tmp - &(list[j][k]));
				if (len == (int)(ft_strlen(list[j]) + 1)
					&& ((!ft_strncmp(list[j], tmp_split[0],
						ft_strlen(tmp_split[0])) && start) || !start)
					&& ((!list[j][k] && end) || !end))
				{
					i = -1;
					*new = ft_charjoin(*new, '\'');
					while (list[j][++i])
						*new = ft_charjoin(*new, list[j][i]);
					*new = ft_charjoin(*new, '\'');
					*new = ft_charjoin(*new, ' ');
					break ;
				}
			}
		}
	}
	free_matrix(list);
	free_matrix(tmp_split);
	(void)shell;
}

void	replace_wild(char **cmd, t_shell *shell)
{
	int		i;
	int		j;
	char	*tmp;
	char	*new;

	i = count_quotes(*cmd);
	shell->words = split_cmd(*cmd, i, shell->pipe);
	j = -1;
	free(*cmd);
	*cmd = ft_strdup("");
	while (shell->words[++j])
	{
		i = -1;
		new = ft_strdup("");
		tmp = ft_strdup("");
		while (shell->words[j][++i])
		{
			if (shell->words[j][i] == '*')
			{
				wild(shell, &new, &shell->words[j], i);
				break ;
			}
			else
				tmp = ft_charjoin(tmp, shell->words[j][i]);
		}
		if (shell->words[j][0] && shell->words[j][i] != '*')
		{
			i = -1;
			while (tmp[++i])
				*cmd = ft_charjoin(*cmd, tmp[i]);
		}
		else if (shell->words[j][0])
		{
			i = -1;
			while (new[++i])
				*cmd = ft_charjoin(*cmd, new[i]);
		}
		*cmd = ft_charjoin(*cmd, ' ');
		free(tmp);
		free(new);
	}
	free_matrix(shell->words);
}

int	process_cmd(char **cmd, t_shell *shell)
{
	int		quotes_count;

	replace_env(cmd, shell);
	replace_wild(cmd, shell);
	quotes_count = count_quotes(*cmd);
	shell->words = split_cmd(*cmd, quotes_count, shell->pipe);
	if (shell->pipe)
		free(shell->pipe);
	debug(*cmd, shell->words, quotes_count);
	if (process_programs(shell))
		shell->pipe = ft_strdup("");
	return (free_matrix(shell->words));
}
