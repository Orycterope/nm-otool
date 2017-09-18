/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/15 14:00:45 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/18 14:17:47 by tvermeil         ###   ########.fr       */
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
	if ((size_t)R(command->stroff) + R(command->strsize) > mapping->size)
	{
		ft_putendl_fd("This file seems to have been truncated", 2);
		return ;
	}
	if (R(((struct mach_header *)mapping->addr)->magic) == MH_MAGIC_64)
		//TODO check command->symoff + command->nsyms * size_64 > mapping->size
		parse_symtab_64(mapping->addr + R(command->symoff), R(command->nsyms),
				mapping->addr + R(command->stroff), *mapping);
	else
		//TODO check command->symoff + command->nsyms * size_32 > mapping->size
		parse_symtab_32(mapping->addr + R(command->symoff), R(command->nsyms),
				mapping->addr + R(command->stroff), *mapping);
}

static void ar_iter_callback(t_list *elem)
{
	t_ar_file		*file;
	t_file_map		mapping;
	unsigned int	magic;

	file = (t_ar_file *)elem->content;
	mapping.addr = file->addr;
	mapping.size = file->size;
	magic = *(unsigned int *)(mapping.addr);
	if (!(magic == MH_MAGIC || magic == MH_CIGAM
		|| magic == MH_MAGIC_64 || magic == MH_CIGAM_64))
		return ;
	ft_printf("%s(%s):\n", file->archive_name, file->filename);
	get_load_commands(mapping, LC_SYMTAB, symtab_command_callback, &mapping);
	if (elem->next != NULL)
		ft_putchar('\n');
}

static int	nm_file(char *filename, int multiple_args)
{
	t_file_map	mapping;
	t_file_map	real_mapping;
	t_list		*ar_file_lst;

	if ((mapping = map_filename(filename)).addr == NULL)
		return (EXIT_FAILURE);
	real_mapping = mapping;
	if ((mapping = get_fat_entry(mapping)).addr == NULL)
		return (EXIT_FAILURE);
	if (check_is_ar_file(mapping.addr))
	{
		ar_file_lst = create_ar_lst(mapping, filename);
		ft_lstiter(ar_file_lst, ar_iter_callback);
		ft_lstdel(&ar_file_lst, free_ar_file);
	}
	else
	{
		if (multiple_args)
			ft_printf("%s:\n", filename);
		get_load_commands(mapping, LC_SYMTAB, symtab_command_callback,
				&mapping);
	}
	unmap_mapping(real_mapping);
	return (EXIT_SUCCESS);
}

int			main(int ac, char *av[])
{
	int	multiple_args;

	multiple_args = ac > 2;
	if (ac < 2)
	{
		ERROR("An argument is needed");
		return (EXIT_FAILURE);
	}
	if (multiple_args)
		ft_putchar('\n');
	while (ac-- - 1) {
		nm_file((av++ + 1)[0], multiple_args);
		if (ac != 1)
			ft_putchar('\n');
	}
	return (EXIT_SUCCESS);
}
