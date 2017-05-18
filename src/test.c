/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 19:10:00 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/18 17:47:56 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int		g_wrong_endian = 0;

int		main(int ac, char *av[])
{
	t_list		*ar_file_lst;
	t_file_map	mapping;

	if (ac < 2)
	{
		ERROR("An argument is needed");
		return (EXIT_FAILURE);
	}
	if ((mapping = map_filename(av[1])).addr == NULL)
		return (EXIT_FAILURE);
	if ((mapping = get_fat_entry(mapping)).addr == NULL)
		return (EXIT_FAILURE);
	if (check_is_ar_file(mapping.addr))
	{
		ft_putendl("This is an ar file");
		ar_file_lst = create_ar_lst(mapping);
		ft_lstdel(&ar_file_lst, free_ar_file);
	}
	return (EXIT_SUCCESS);
}
