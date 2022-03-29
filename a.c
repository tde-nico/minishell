/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 13:47:21 by tde-nico          #+#    #+#             */
/*   Updated: 2022/03/29 13:47:21 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int	main(void)
{
	char	b;

	write(1, "input: ", 7);
	while (read(0, &b, 1) && b != 'q')
		;
	if (b == 'q')
		printf("quit\n");
	else
		printf("test\n");
	return (0);
}
