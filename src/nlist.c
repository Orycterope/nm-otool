/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 17:52:25 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/24 18:59:03 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include <mach-o/nlist.h>
#include <mach-o/loader.h>

/*
** The functions of this file are used to parse and display the symtab of the
** Mach-o file.
*/

void	parse_symtab_32(struct nlist *tab, int n_sym, char *str_tab,
	t_file_map	file)
{
	(void)file;
	while (n_sym--)
	{
		if ((tab->n_type & 0xe0) == 0)
		{
			ft_printf("%016lx %c %s\n", tab->n_value,
				'*',
				str_tab + tab->n_un.n_strx);
		tab++;
		}
	}
}

void	parse_symtab_64(struct nlist_64 *tab, int n_sym, char *str_tab,
	t_file_map file)
{
	struct section	*sect;

	while (n_sym--)
	{
		if ((tab->n_type & 0xe0) == 0)
		{
			if (tab->n_sect != NO_SECT)
			{
				sect = get_section_by_number(file, tab->n_sect);
				ft_printf("%016lx %c %s\t%s %s\n", tab->n_value,
					'+',
					str_tab + tab->n_un.n_strx,
					sect->segname, sect->sectname);
			}
			else
			{
				ft_printf("%016lx %c %s\n", tab->n_value,
					'-',
					str_tab + tab->n_un.n_strx);
			}
		}
		tab++;
	}
}
