/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 17:52:25 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/14 10:44:26 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/nlist.h>
#include <mach-o/loader.h>

/*
** The functions of this file are used to parse the symtab of the Mach-o file.
*/

static int	sort_func(void *c1, void *c2)
{
	t_symbol	*sym1;
	t_symbol	*sym2;

	sym1 = (t_symbol *)c1;
	sym2 = (t_symbol *)c2;
	return (ft_strcmp(sym1->name, sym2->name));
}

static void	del_func(void *content, size_t s)
{
	(void)s;
	free(content);
}

void		parse_symtab_32(struct nlist *tab, int n_sym, char *str_tab,
	t_file_map file)
{
	t_list		*symlst;
	t_symbol	content;
	t_list		*elem;

	symlst = NULL;
	while (n_sym--)
	{
		content.data = tab;
		content.name = str_tab + R(tab->n_un.n_strx);
		content.file = file;
		elem = ft_lstnew(&content, sizeof(content));
		ft_lstappend(&symlst, elem);
		tab++;
	}
	ft_lstsort(symlst, sort_func);
	ft_lstiter(symlst, print_sym);
	ft_lstdel(&symlst, del_func);
}

/*
** Save the symtab as a list, sort it, and display it line by line
*/

void		parse_symtab_64(struct nlist_64 *tab, int n_sym, char *str_tab,
	t_file_map file)
{
	t_list		*symlst;
	t_symbol	content;
	t_list		*elem;

	symlst = NULL;
	while (n_sym--)
	{
		content.data = tab;
		content.name = str_tab + R(tab->n_un.n_strx);
		content.file = file;
		elem = ft_lstnew(&content, sizeof(content));
		ft_lstappend(&symlst, elem);
		tab++;
	}
	ft_lstsort(symlst, sort_func);
	ft_lstiter(symlst, print_sym_64);
	ft_lstdel(&symlst, del_func);
}
