/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 15:28:48 by maperrea          #+#    #+#             */
/*   Updated: 2020/01/28 12:13:41 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEAK_DETECTOR_H
# define LEAK_DETECTOR_H

# ifdef CHECK_LEAKS

#  include <unistd.h>
#  include <string.h>
#  include <stdio.h>
#  include <stdlib.h>

void	*malloc_leak(size_t size, const char *file, int line);
void	free_leak(void *ptr);
void	print_leaks();

#  define malloc(X) malloc_leak(X, __FILE__, __LINE__)
#  define free(X)  free_leak(X)
#  define CHECK print_leaks();

# else
#  define CHECK
# endif

#endif
