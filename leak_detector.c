/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 16:23:50 by maperrea          #+#    #+#             */
/*   Updated: 2020/01/23 14:58:22 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef VERBOSE
# define VERBOSE 1
#else
# define VERBOSE 0
#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct	s_malloc
{
	void			*ptr;
	size_t			size;
	const char		*file;
	int				line;
	struct s_malloc	*next;
}				t_malloc;

static t_malloc *g_malloc_list;

void	*malloc_leak(size_t size, const char *file, int line)
{
	void		*ptr;
	t_malloc	*new_malloc;
	t_malloc	*tmp;

	tmp = g_malloc_list;
	if (!(ptr = malloc(size)))
		return (NULL);
	if (!(new_malloc = malloc(sizeof(t_malloc))))
		return (NULL);
	new_malloc->ptr = ptr;
	new_malloc->size = size;
	new_malloc->file = file;
	new_malloc->line = line;
	new_malloc->next = NULL;
	if (VERBOSE)
		printf(">>> malloc in file %s at line %d at adress %p\n",
				new_malloc->file, new_malloc->line, new_malloc->ptr);
	if (!g_malloc_list)
		g_malloc_list = new_malloc;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_malloc;
	}
	return (ptr);
}

void	free_leak(void *ptr)
{
	t_malloc	*next;
	t_malloc	*tmp;

	if (!ptr)
		return ;
	tmp = g_malloc_list;
	if (tmp->ptr == ptr)
	{
		next = tmp->next;
		if (VERBOSE)
			printf(">>> free of malloc from file %s at line %d at address %p\n",
					tmp->file, tmp->line, ptr);
		free(ptr);
		free(tmp);
		g_malloc_list = next;
	}
	else
	{
		while (tmp->next->ptr != ptr)
			tmp = tmp->next;
		next = tmp->next->next;
		if (VERBOSE)
			printf(">>> free of malloc from file %s at line %d at address %p\n",
					tmp->next->file, tmp->next->line, ptr);
		free(ptr);
		free(tmp->next);
		tmp->next = next;
	}
}

void		print_leaks()
{
	if (!g_malloc_list)
	{
		printf(">>> No leaks!\n>>> Well played!\n");
	}
	while(g_malloc_list)
	{
		printf("\n++++++++++++\npointer %p of size %li allocated in file %s at line %d leaked\n",
g_malloc_list->ptr, g_malloc_list->size, g_malloc_list->file, g_malloc_list->line);
		printf("content: %s\n++++++++++++\n", (char *)g_malloc_list->ptr);
		g_malloc_list = g_malloc_list->next;
	}
}
