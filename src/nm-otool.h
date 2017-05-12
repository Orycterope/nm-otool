/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm-otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 16:56:15 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/12 19:38:35 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include "libft.h"

# define error(str)		ft_putendl_fd(str, 2)

/*
** map.c
*/
void	*map_filename(const char *pathname);

/*
** fat.c
*/
void	*get_fat_entry(void *map_addr);

#endif
