/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:01:31 by tde-nico          #+#    #+#             */
/*   Updated: 2022/05/18 09:09:52 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #####################  cmd execution  #####################

int	execute(int fd, t_shell *shell)
{
	int		status;

	status = 0;
	if (shell->mode[shell->fix] && ft_strchr("|>C", shell->mode[shell->fix]))
		dup2(fd, 1);
	if (execve(shell->words[0], shell->words, shell->env))
		status = 127;
	write(fd, "\0", 1);
	free_shell(shell, 1);
	return (status);
}

void	execute_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	pipe(fd);
	signal(SIGINT, handle_child_sigint);
	pid = fork();
	if (pid == -1)
		exit(1 + 0 * ft_printf("Fork Error\n"));
	if (!pid)
	{
		close(fd[0]);
		exit(execute(fd[1], shell));
	}
	waitpid(pid, &status, 0);
	close(fd[1]);
	free(shell->exit_code);
	shell->exit_code = ft_itoa(WEXITSTATUS(status));
	get_pipe_exit(fd[0], shell);
}
