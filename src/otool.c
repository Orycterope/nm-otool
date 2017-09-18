/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/15 13:32:54 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/18 18:25:12 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/loader.h>

int		g_wrong_endian = 0;

static void	hexdump_mapping(t_sect_minimal sect)
{
	int		c;

	while (sect.size > 0)
	{
		c = -1;
		if (sect.is_32)
			ft_printf("%08x\t", sect.dest);
		else
			ft_printf("%016lx\t", sect.dest);
		while (++c < 16 && sect.size > 0)
		{
			ft_printf("%02x ", *((unsigned char *)sect.addr));
			sect.addr++;
			sect.dest++;
			sect.size--;
		}
		ft_printf("\n");
	}
}

static void	print_section(t_file_map mapping)
{
	t_sect_minimal	sect;

	sect = get_section_by_name(mapping, SEG_TEXT, SECT_TEXT);
	if (sect.addr == NULL)
	{
		ft_printf("No section " SECT_TEXT " found.\n");
		return ;
	}
	else if (sect.addr > mapping.addr + mapping.size)
	{
		ft_printf("The file seems to have been truncated.\n");
		return ;
	}
	ft_printf("Contents of (%s,%s) section\n", SEG_TEXT, SECT_TEXT);
	hexdump_mapping(sect);
}

static void	ar_iter_callback(t_list *elem)
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
	print_section(mapping);
}

static int	otool_file(char *filename)
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
		ft_printf("Archive : %s\n", filename);
		ar_file_lst = create_ar_lst(mapping, filename);
		ft_lstiter(ar_file_lst, ar_iter_callback);
		ft_lstdel(&ar_file_lst, free_ar_file);
	}
	else
	{
		ft_printf("%s:\n", filename);
		print_section(mapping);
	}
	unmap_mapping(real_mapping);
	return (EXIT_SUCCESS);
}

int			main(int ac, char *av[])
{
	if (ac < 2)
	{
		ERROR("An argument is needed");
		return (EXIT_FAILURE);
	}
	while (ac-- - 1)
		otool_file((av++ + 1)[0]);
	return (EXIT_SUCCESS);
}
