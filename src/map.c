/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 16:55:15 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/12 19:40:10 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm-otool.h"
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
** returns the address to the mapping
** on error this function displays a small text and call exit.
*/
void	*map_filename(const char *pathname)
{
	int			fd;
	void		*ptr;
	struct stat	my_stat;

	if ((fd = open(pathname, O_RDONLY)) < 0)
	{
		error("The file does not exist or cannot be opened");
		return (NULL);
	}
	if (fstat(fd, &my_stat) != 0)
	{
		error("The file cannot be stat'd");
		return (NULL);
	}
	ptr = mmap(NULL, my_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		error("The file cannot be mmap'd");
		return (NULL);
	}
	close(fd);
	return (ptr);
}
