/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/15 18:01:36 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/15 18:10:34 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENDIAN_H
# define ENDIAN_H

# define R(n)	(g_wrong_endian ? ft_reverse_bytes_to_long(&n, sizeof(n)) : n)

extern int	g_wrong_endian;

#endif
