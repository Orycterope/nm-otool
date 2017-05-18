/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm-otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 16:56:15 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/18 19:19:17 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include "libft.h"

# define ERROR(str)	ft_putendl_fd(str, 2)
# define AR_MAGIC "!<arch>\x0a"
# define AR_CIGAM "\x0a>hcra<!"

/*
** A structure to store a pointer to a file and its size
*/
typedef struct	s_file_map
{
	void		*addr;
	size_t		size;
}				t_file_map;

/*
** A structure to make a list of files in an ar file
*/
typedef struct	s_ar_file
{
	char		*filename;
	void		*addr;
	size_t		size;
}				t_ar_file;

/*
** map.c
*/
t_file_map		map_filename(const char *pathname);

/*
** fat.c
*/
t_file_map		get_fat_entry(t_file_map mapping);

/*
** ar.c
*/
int				check_is_ar_file(void *addr);
t_list			*create_ar_lst(t_file_map archive);
void			free_ar_file(void *content, size_t size);

#endif
