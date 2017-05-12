/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 19:10:00 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/12 19:14:30 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm-otool.h"

int		main(int ac, char *av[])
{
	char	*ptr;
	if (ac < 2)
	{
		error("An argument is needed");
		return (EXIT_FAILURE);
	}
	if ((ptr = map_filename(av[1])) == NULL)
		return (EXIT_FAILURE);
	if ((ptr = get_fat_entry(ptr)) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
