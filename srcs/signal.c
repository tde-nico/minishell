/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 13:33:54 by tde-nico          #+#    #+#             */
/*   Updated: 2022/03/29 13:33:54 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	ft_printf("\n%s", PROMPT);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	ft_printf("\n%s", PROMPT);
}

void	handle_child_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}
