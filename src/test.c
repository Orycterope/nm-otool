/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 19:10:00 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/15 19:59:40 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int		g_wrong_endian = 0;

int		main(int ac, char *av[])
{
	char	*ptr;

	if (ac < 2)
	{
		ERROR("An argument is needed");
		return (EXIT_FAILURE);
	}
	if ((ptr = map_filename(av[1])) == NULL)
		return (EXIT_FAILURE);
	if ((ptr = get_fat_entry(ptr)) == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
