/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 13:26:42 by tde-nico          #+#    #+#             */
/*   Updated: 2022/03/29 13:26:42 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_home_path(t_shell *shell, char **path)
{
	int	i;

	i = -1;
	(void)path;
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], "HOME=", 5))
		{
			*path = ft_strdup(&shell->env[i][5]);
			return (0);
		}
	}
	return (ft_printf("cd: HOME not set\n"));
}

int	get_old_path(t_shell *shell, char **path)
{
	int		i;

	i = -1;
	(void)path;
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], "OLDPWD=", 7))
		{
			*path = ft_strdup(&shell->env[i][7]);
			return (0);
		}
	}
	return (ft_printf("cd: OLDPWD not set\n"));
}

int	update_path(t_shell *s, char *p)
{
	int		old_pwd;
	int		pwd;
	char	*tmp;
	char	*path;
	int		i;

	i = -1;
	old_pwd = -1;
	pwd = -1;
	path = NULL;
	while (s->env[++i])
	{
		if (!ft_strncmp(s->env[i], "OLDPWD=", 7))
			old_pwd = i;
		else if (!ft_strncmp(s->env[i], "PWD=", 4))
			pwd = i;
	}
	if (pwd == -1)
		return (ft_printf("cd: PWD not set\n"));
	if (old_pwd == -1)
		return (ft_printf("cd: OLDPWD not set\n"));
	tmp = s->env[old_pwd];
	s->env[old_pwd] = ft_strjoin("OLDPWD=", &s->env[pwd][4]);
	free(tmp);
	tmp = s->env[pwd];
	path = getcwd(path, ft_strlen(s->path) + ft_strlen(p) + 2);
	s->env[pwd] = ft_strjoin("PWD=", path);
	free(s->path);
	s->path = ft_strdup(path);
	free(tmp);
	free(path);
	return (0);
}

int	change_dir(t_shell *shell)
{
	char	*path;

	path = NULL;
	if (!shell->words[1] && get_home_path(shell, &path))
		return (1);
	if (!path && shell->words[1] && shell->words[2])
		return (1 + 0 * ft_printf("cd: too many arguments\n"));
	if (!path && !ft_strncmp(shell->words[1], "-", 2)
		&& get_old_path(shell, &path))
		return (1);
	if (!path)
		path = ft_strdup(shell->words[1]);
	if (chdir(path) == -1)
	{
		ft_printf("cd: %s: No such file or directory\n", path);
		free(path);
		return (1);
	}
	else
		update_path(shell, path);
	free(path);
	return (0);
}

void	cd(t_shell *shell)
{
	shell->pipe = ft_strdup("");
	free(shell->exit_code);
	shell->exit_code = ft_itoa(change_dir(shell));
}
