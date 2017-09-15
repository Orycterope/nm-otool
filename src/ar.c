/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 15:17:58 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/15 13:22:20 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <ranlib.h>

#define AR_MAGIC_SIZE 8
#define AR_FILE_NAME_SIZE 16
#define AR_FILE_SIZE_OFFSET 48
#define AR_FILE_CONTENT_OFFSET 60

/*
** Checks if the file is an ar file by searching for the !<arch> string
** Sets the g_wrong_endian.
*/

int					check_is_ar_file(void *addr)
{
	if (ft_memcmp(addr, AR_MAGIC, AR_MAGIC_SIZE) == 0)
		return (1);
	return (0);
}

/*
** Gets the long filename stored at the beginning of the file
** moves the addr ptr, and reduces the file.size.
*/

static void			extract_name(t_ar_file *f)
{
	int	name_len;

	if (ft_memcmp(f->filename, "#1/", 3) == 0)
	{
		name_len = ft_atoi(f->filename + 3);
		if (name_len <= 0)
			return ;
		free(f->filename);
		f->filename = malloc(name_len + 1);
		ft_strncpy(f->filename, f->addr, name_len);
		f->filename[name_len] = '\0';
		f->addr += name_len;
		f->size -= name_len;
	}
}

/*
** Fills an t_ar_file with the information in the ar headers
*/

static t_ar_file	extract_info(void *addr)
{
	t_ar_file	file;
	char		name_buf[AR_FILE_NAME_SIZE + 1];
	char		*trimer;

	name_buf[16] = '\0';
	ft_memcpy(name_buf, (char *)addr, 16);
	trimer = &name_buf[AR_FILE_NAME_SIZE - 1];
	while (*trimer == ' ')
		*trimer-- = '\0';
	file.filename = ft_strdup(name_buf);
	file.addr = addr + AR_FILE_CONTENT_OFFSET;
	file.size = (size_t)ft_atoi(addr + AR_FILE_SIZE_OFFSET);
	return (file);
}

/*
** Called to create a list of the files in the ar archive.
** If the file is not an archive, returns NULL.
*/

t_list				*create_ar_lst(t_file_map archive, char *archive_name)
{
	t_list		*file_lst;
	size_t		this_file_size;
	t_list		*elem;
	t_ar_file	file;

	if (!check_is_ar_file(archive.addr))
		return (NULL);
	file_lst = NULL;
	archive.addr += AR_MAGIC_SIZE;
	archive.size -= AR_MAGIC_SIZE;
	while (archive.size)
	{
		file = extract_info(archive.addr);
		this_file_size = file.size;
		extract_name(&file);
		file.archive_name = archive_name;
		elem = ft_lstnew(&file, sizeof(file));
		if (elem == NULL) //
			break ; //
		ft_lstappend(&file_lst, elem);
		archive.addr += AR_FILE_CONTENT_OFFSET + this_file_size;
		archive.size -= AR_FILE_CONTENT_OFFSET + this_file_size;
	}
	return (file_lst);
}

/*
** Callback function when freeing the ar list
*/

void				free_ar_file(void *content, size_t size)
{
	t_ar_file	*f;

	(void)size;
	f = (t_ar_file *)content;
	free(f->filename);
	free(content);
}
