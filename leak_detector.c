/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 16:23:50 by maperrea          #+#    #+#             */
/*   Updated: 2020/02/25 14:08:06 by maperrea         ###   ########.fr       */
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

static t_malloc *__malloc_list;

void	*__malloc_leak(size_t size, const char *file, int line, const char *function)
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
		printf("\033[38;5;1m>>>\033[0m \033[38;5;5mmalloc\033[0m in file \033[38;5;5m%s\033[0m at \033[38;5;5mline %d\033[0m in function \033[38;5;5m%s\033[0m at address %p\n",
				file, line, function, ptr);
	if (!__malloc_list)
		__malloc_list = new_malloc;
	else
	{
		tmp = __malloc_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_malloc;
	}
	return (ptr);
}

void	__free_leak(void *ptr, const char *file, int line, const char *function)
{
	t_malloc	*next;
	t_malloc	*tmp;

	if (!ptr)
		return ;
	tmp = __malloc_list;
	if (tmp->ptr == ptr)
	{
		next = tmp->next;
		if (VERBOSE)
			printf("\033[38;5;1m>>>\033[0m \033[38;5;33mfree\033[0m in file \033[38;5;33m%s\033[0m at \033[38;5;33mline %d\033[0m in function \033[38;5;33m%s\033[0m of malloc from file \033[38;5;33m%s\033[0m at \033[38;5;33mline %d\033[0m in function \033[38;5;33m%s\033[0m at address %p\n", file, line, function, tmp->file, tmp->line, tmp->function, ptr);
		free(ptr);
		free(tmp);
		__malloc_list = next;
	}
	else
	{
		while (tmp->next->ptr != ptr)
			tmp = tmp->next;
		next = tmp->next->next;
		if (VERBOSE)
			printf("\033[38;5;1m>>>\033[0m \033[38;5;33mfree\033[0m in file \033[38;5;33m%s\033[0m at \033[38;5;33mline %d\033[0m in function \033[38;5;33m%s\033[0m of malloc from file \033[38;5;33m%s\033[0m at \033[38;5;33mline %d\033[0m in function \033[38;5;33m%s\033[0m at address %p\n", file, line, function, tmp->next->file, tmp->next->line, tmp->next->function, ptr);
		free(ptr);
		free(tmp->next);
		tmp->next = next;
	}
}

void		__print_leaks()
{
	if (!__malloc_list)
	{
		printf("\033[38;5;2m>>> No leaks!\n>>> Well played!\033[0m\n");
	}
	while(__malloc_list)
	{
		printf("\n++++++++++++\npointer %p of size %li allocated in file \033[38;5;1m%s\033[0m at \033[38;5;1mline %d\033[0m in function \033[38;5;1m%s\033[m leaked\n", __malloc_list->ptr, __malloc_list->size, __malloc_list->file, __malloc_list->line, __malloc_list->function);
		printf("content:\n\033[38;5;2m%.*s\033[0m\n++++++++++++\n", (int)__malloc_list->size, (char *)__malloc_list->ptr);
		__malloc_list = __malloc_list->next;
	}
}
