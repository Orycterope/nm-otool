/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 17:52:25 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/26 19:42:27 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/nlist.h>
#include <mach-o/loader.h>

/*
** The functions of this file are used to parse and display the symtab of the
** Mach-o file.
*/

/*
** Gets the nm letter to be displayed
*/
static char	get_letter(uint8_t n_type, uint8_t n_sect, t_file_map file)
{
	char			letter;
	struct section	*sect;
	int				local;

	(void)file; //
	local = ((n_type & N_EXT) == 0);
	n_type &= N_TYPE;
	if (n_type == N_UNDF)
		letter = 'U';
	else if (n_type == N_ABS)
		letter = 'A';
	else if (n_type == N_SECT)
	{
		letter = 'S';
		sect = get_section_by_number(file, n_sect);
		if (ft_strncmp(sect->segname, SEG_TEXT, 16) == 0
			&& ft_strncmp(sect->sectname, SECT_TEXT, 16) == 0)
			letter = 'T';
		else if (ft_strncmp(sect->segname, SEG_DATA, 16) == 0
			&& ft_strncmp(sect->sectname, SECT_DATA, 16) == 0)
			letter = 'D';
		else if (ft_strncmp(sect->segname, SEG_DATA, 16) == 0
			&& ft_strncmp(sect->sectname, SECT_BSS, 16) == 0)
			letter = 'B';
		else if (ft_strncmp(sect->segname, SEG_DATA, 16) == 0
			&& ft_strncmp(sect->sectname, SECT_COMMON, 16) == 0)
			letter = 'C'; // see nlist.h:161
	}
	else if (n_type == N_INDR)
		letter = 'I';
	else if (n_type == N_PBUD)
		letter = 'P'; //
	else
		letter = '?';
	return (local ? letter + ('a' - 'A') : letter);
}

void	parse_symtab_32(struct nlist *tab, int n_sym, char *str_tab,
	t_file_map	file)
{
	(void)file;
	while (n_sym--)
	{
		if ((R(tab->n_type) & 0xe0) == 0)
		{
			ft_printf("%08lx %c %s\n", R(tab->n_value),
				'*',
				str_tab + R(tab->n_un.n_strx));
		}
		tab++;
	}
}

void	parse_symtab_64(struct nlist_64 *tab, int n_sym, char *str_tab,
	t_file_map file)
{
	//struct section	*sect;

	while (n_sym--)
	{
		if ((R(tab->n_type) & 0xe0) == 0)
		{
			if (R(tab->n_value) == 0) // && UNDEFINED
			{
					ft_printf("%16c %c %s\n",
					' ',
					get_letter(R(tab->n_type), R(tab->n_sect), file),
					str_tab + R(tab->n_un.n_strx));

			}
			/*else if (R(tab->n_sect) != NO_SECT)
			{
				sect = get_section_by_number(file, R(tab->n_sect));
				ft_printf("%016lx %c %c %s\t%s %s\n",
					R(tab->n_value),
					'+', get_letter(R(tab->n_type), file),
					str_tab + R(tab->n_un.n_strx),
					sect->segname, sect->sectname);
			}*/
			else
			{
				ft_printf(
					//"%016lx %0#4lx %c %s\n",
					"%016lx %c %s\n",
					R(tab->n_value),
					//tab->n_type, //
					get_letter(R(tab->n_type), R(tab->n_sect), file),
					str_tab + R(tab->n_un.n_strx));
			}
		}
		tab++;
	}
}
