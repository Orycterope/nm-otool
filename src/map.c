/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 16:55:15 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/15 12:35:03 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

/*
** This file contains the functions used to map the file to memory
*/

/*
** This functions tries to open -pathname-, fstat it to get its size,
** map it and finally close the fd.
** returns the address to the mapping and its size.
** on error this function displays a small text and sets ret.addr to NULL
*/

t_file_map	map_filename(const char *pathname)
{
	int			fd;
	struct stat	my_stat;
	t_file_map	mapping;

	ft_bzero(&mapping, sizeof(t_file_map));
	if ((fd = open(pathname, O_RDONLY)) < 0)
	{
		ft_printf("%s: %s\n", pathname, "The file does not exist or cannot be opened");
		return (mapping);
	}
	if (fstat(fd, &my_stat) != 0)
	{
		ft_printf("%s: %s\n", pathname, "The file cannot be stat'd");
		return (mapping);
	}
	mapping.addr = mmap(NULL, my_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mapping.addr == MAP_FAILED)
	{
		ft_printf("%s: %s\n", pathname, "The file cannot be mmap'd");
		mapping.addr = NULL;
		return (mapping);
	}
	close(fd);
	mapping.size = my_stat.st_size;
	return (mapping);
}

int			unmap_mapping(t_file_map map)
{
	return (munmap(map.addr, map.size));
}
