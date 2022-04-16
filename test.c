/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-nico <tde-nico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 10:52:18 by tde-nico          #+#    #+#             */
/*   Updated: 2022/04/13 11:21:35 by tde-nico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc < 2)
	{
		printf("ERROR\n");
		return (1);
	}
	while (argv[++i])
	{
		printf("arg %d : %s\n", i, argv[i]);
	}
	return (0);
}
