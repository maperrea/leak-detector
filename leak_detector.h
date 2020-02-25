/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 15:28:48 by maperrea          #+#    #+#             */
/*   Updated: 2020/02/25 14:06:17 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEAK_DETECTOR_H
# define LEAK_DETECTOR_H

# ifdef CHECK_LEAKS

#  include <unistd.h>
#  include <string.h>
#  include <stdio.h>
#  include <stdlib.h>

void	*__malloc_leak(size_t size, const char *file, int line, const char *function);
void	__free_leak(void *ptr, const char *file, int line, const char *function);
void	__print_leaks();

#  define malloc(X) __malloc_leak(X, __FILE__, __LINE__, __func__)
#  define free(X)  __free_leak(X, __FILE__, __LINE__, __func__)
#  undef CHECK_LEAKS
#  define CHECK_LEAKS				\
			do {					\
				__print_leaks();	\
			} while (0)

# else
#  define CHECK_LEAKS
# endif

#endif
