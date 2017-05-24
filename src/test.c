/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 19:10:00 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/24 18:27:27 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/loader.h>

int		g_wrong_endian = 0;

static void	symtab_command_callback(void *command_addr, void *data)
{
	struct symtab_command	*command;
	t_file_map				*mapping;

	command = (struct symtab_command *)command_addr;
	mapping = (t_file_map *)data;
	if (command->stroff + command->strsize > mapping->size)
	{
		ft_putendl_fd("This file seems to have been truncated", 2);
		return ;
	}
	if (R(((struct mach_header *)mapping->addr)->magic) == MH_MAGIC_64)
		parse_symtab_64(mapping->addr + command->symoff, command->nsyms,
				mapping->addr + command->stroff, *mapping);
	else
		parse_symtab_32(mapping->addr + command->symoff, command->nsyms,
				mapping->addr + command->stroff, *mapping);
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
	get_load_commands(mapping, LC_SYMTAB, symtab_command_callback, &mapping);
	return (EXIT_SUCCESS);
}
