/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 16:23:50 by maperrea          #+#    #+#             */
/*   Updated: 2020/02/23 21:36:10 by maperrea         ###   ########.fr       */
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
	const char		*function;
	struct s_malloc	*next;
}				t_malloc;

static t_malloc *g_malloc_list;

void	*malloc_leak(size_t size, const char *file, int line, const char *function)
{
	void		*ptr;
	t_malloc	*new_malloc;
	t_malloc	*tmp;

	if (!(ptr = malloc(size)))
		return (NULL);
	if (!(new_malloc = malloc(sizeof(t_malloc))))
		return (NULL);
	new_malloc->ptr = ptr;
	new_malloc->size = size;
	new_malloc->file = file;
	new_malloc->function = function;
	new_malloc->line = line;
	new_malloc->next = NULL;
	if (VERBOSE)
		printf(">>> malloc in file %s at line %d in function %s at address %p\n",
				file, line, function, ptr);
	if (!g_malloc_list)
		g_malloc_list = new_malloc;
	else
	{
		tmp = g_malloc_list;
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
			printf(">>> free of malloc from file %s at line %d in file %s at address %p\n", tmp->file, tmp->line, tmp->function, ptr);
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
			printf(">>> free of malloc from file %s at line %d in file %s at address %p\n", tmp->next->file, tmp->next->line, tmp->next->function, ptr);
		free(ptr);
		free(tmp->next);
		tmp->next = next;
	}
}

void		print_leaks()
{
	if (!g_malloc_list)
	{
		printf("\033[38;5;2m>>> No leaks!\n>>> Well played!\033[0m\n");
	}
	while(g_malloc_list)
	{
		printf("\n++++++++++++\npointer %p of size %li allocated in file \033[38;5;1m%s\033[0m at \033[38;5;1mline %d\033[0m in function \033[38;5;1m%s\033[m leaked\n", g_malloc_list->ptr, g_malloc_list->size, g_malloc_list->file, g_malloc_list->line, g_malloc_list->function);
		printf("content: %.*s\n++++++++++++\n", (int)g_malloc_list->size, (char *)g_malloc_list->ptr);
		g_malloc_list = g_malloc_list->next;
	}
}
