/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 19:10:00 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/19 18:59:47 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include <mach-o/loader.h>

int		g_wrong_endian = 0;

static void	callback(void *addr)
{
	(void)addr;
	ft_putendl("I'm a callback function ! :D");
}

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
	get_load_commands(mapping, LC_SYMTAB, callback);
	return (EXIT_SUCCESS);
}
