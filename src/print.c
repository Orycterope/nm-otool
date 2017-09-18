/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 10:31:56 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/18 18:35:49 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/nlist.h>
#include <mach-o/loader.h>

/*
** The function of this file are used to print a nm line based on the symtab
*/

static char	get_letter_by_sect(uint8_t n_sect, t_file_map file)
{
	char			letter;
	struct section	*sect;

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
	return (letter);
}

/*
** Gets the nm letter to be displayed
*/

static char	get_letter(uint8_t n_type, uint8_t n_sect, int value_is_null,
		t_file_map file)
{
	char			letter;
	int				local;

	local = ((n_type & N_EXT) == 0);
	n_type &= N_TYPE;
	if (n_type == N_UNDF && n_sect == NO_SECT && !value_is_null)
		letter = 'C';
	else if (n_type == N_UNDF)
		letter = 'U';
	else if (n_type == N_ABS)
		letter = 'A';
	else if (n_type == N_SECT)
		letter = get_letter_by_sect(n_sect, file);
	else if (n_type == N_INDR)
		letter = 'I';
	else if (n_type == N_PBUD)
		letter = 'P';
	else
		letter = '?';
	return (local ? letter + ('a' - 'A') : letter);
}

void		print_sym(t_list *elem)
{
	t_symbol		*symbol_struct;
	struct nlist	*sym;

	symbol_struct = elem->content;
	sym = symbol_struct->data;
	if ((R(sym->n_type) & 0xe0) == 0)
	{
		if (R(sym->n_value) == 0 && (R(sym->n_type) & N_TYPE) == N_UNDF)
		{
			ft_printf("%8c %c %s\n",
			' ',
			get_letter(R(sym->n_type), R(sym->n_sect), R(sym->n_value) == 0,
				symbol_struct->file),
			symbol_struct->name);
		}
		else
		{
			ft_printf(
				"%08x %c %s\n",
				R(sym->n_value),
				get_letter(R(sym->n_type), R(sym->n_sect), R(sym->n_value) == 0,
					symbol_struct->file),
				symbol_struct->name);
		}
	}
}

void		print_sym_64(t_list *elem)
{
	t_symbol		*symbol_struct;
	struct nlist_64	*sym;

	symbol_struct = elem->content;
	sym = symbol_struct->data;
	if ((R(sym->n_type) & 0xe0) == 0)
	{
		if (R(sym->n_value) == 0 && (R(sym->n_type) & N_TYPE) == N_UNDF)
		{
			ft_printf("%16c %c %s\n",
			' ',
			get_letter(R(sym->n_type), R(sym->n_sect), R(sym->n_value) == 0,
				symbol_struct->file),
			symbol_struct->name);
		}
		else
		{
			ft_printf(
				"%016lx %c %s\n",
				R(sym->n_value),
				get_letter(R(sym->n_type), R(sym->n_sect), R(sym->n_value) == 0,
					symbol_struct->file),
				symbol_struct->name);
		}
	}
}
