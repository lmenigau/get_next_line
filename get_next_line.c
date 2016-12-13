/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenigau <lmenigau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:05:57 by lmenigau          #+#    #+#             */
/*   Updated: 2016/12/13 21:57:45 by lmenigau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file	*find_or_create_struct(t_list **files, int fd)
{
	t_file	*file;
	t_list	*prevfile;
	t_list	*current;
	t_file	*newfile;

	current = *files;
	while (current != NULL)
	{
		file = current->content;
		if (file->fd == fd)
			return (file);
		current = current->next;
	}
	newfile = ft_memalloc(sizeof(t_file));
	newfile->fd = fd;
	prevfile = *files;
	*files = malloc(sizeof(**files));
	(*files)->content = newfile;
	(*files)->next = prevfile;
	return (newfile);
}

int		extract_line(t_file *file, char **line, char *buff, size_t size)
{
	char	*nl;
	char	*to_free;

	if ((nl = ft_memchr(buff, '\n', size)))
	{
		*nl = '\0';
		*line = ft_memjoin(NULL, buff, 0, nl - buff + 1);
		file->size = size - (nl - buff + 1);
		to_free = file->rest;
		file->rest = ft_memjoin(nl + 1, NULL, file->size, 0);
		free(to_free);
		if (file->rest == NULL || *line == NULL)
			return (-1);
		return (1);
	}
	return (0);
}

int		manage_file(t_file *file, char **line)
{
	int		byte_read;
	char	*to_free;
	char	*buff;
	int		ret;

	if (file->size && (ret = extract_line(file, line, file->rest, file->size)))
		return (ret);
	if ((buff = malloc(BUFF_SIZE)) == NULL)
		return (-1);
	while ((byte_read = read(file->fd, buff, BUFF_SIZE)) > 0)
	{
		to_free = file->rest;
		file->rest = ft_memjoin(file->rest, buff, file->size, byte_read);
		free(to_free);
		file->size += byte_read;
		if ((ret = extract_line(file, line, file->rest, file->size)))
		{
			free(buff);
			return (ret);
		}
	}
	free(buff);
	return (byte_read);
}

int		get_next_line(const int fd, char **line)
{
	t_file			*file;
	static t_list	*files = NULL;
	int				ret;

	if (line == NULL || fd < 0)
		return (-1);
	if (files == NULL)
	{
		files = malloc(sizeof(t_list));
		file = ft_memalloc(sizeof(t_file));
		file->fd = fd;
		files->content = file;
	}
	file = find_or_create_struct(&files, fd);
	ret = manage_file(file, line);
	if (file->size && !ret)
	{
		*line = ft_memjoin(NULL, file->rest, 0, file->size + 1);
		(*line)[file->size] = '\0';
		file->size = 0;
		return (1);
	}
	return (ret);
}
