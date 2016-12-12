/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenigau <lmenigau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:05:57 by lmenigau          #+#    #+#             */
/*   Updated: 2016/12/12 19:19:14 by lmenigau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file	*find_or_create_struct(t_list *files, int fd)
{
	t_file	*file;
	t_list	*prevfile;
	t_file	*newfile;

	prevfile = NULL;
	while (files != NULL && files->content)
	{
		file = files->content;
		if (file->fd == fd)
			return (file);
		prevfile = files;
		files = files->next;
	}
	if ((newfile = malloc(sizeof (t_file))) == NULL)
		return (NULL);
	newfile->fd = fd;
	newfile->rest = NULL;
	newfile->size = 0;
	if (prevfile)
		prevfile->next->content = newfile;
	else
		files->content = newfile;
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
		file->size = size - (nl - buff);
		to_free = file->rest;
		file->rest = ft_memjoin(nl + 1, NULL, file->size, 0);
		free(to_free);
		if (file->rest == NULL)
			return (-1);
		return (1);
	}
	return (0);
}

int		manage_file(t_file *file, char **line)
{
	int		byte_read;
	char	*buff;

	if (file->size != 0)
	{
		if (extract_line(file, line, file->rest, file->size) == 1)
			return (1);
	}
	buff = malloc(BUFF_SIZE);
	while ((byte_read = read(file->fd, buff, BUFF_SIZE)) > 0)
	{
		if (extract_line(file, line, buff, byte_read) == 1)
		{
			free(buff);
			return (1);
		}
		else
			file->rest = ft_memjoin(file->rest, buff, file->size, byte_read);
	}
	free(buff);
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	t_file			*file;
	static t_list	*files = NULL;

	if (files == NULL && (files = ft_memalloc(sizeof *files)) == NULL)
		return (-1);
	file = find_or_create_struct(files, fd);
	if (manage_file(file, line) == 1)
		return (1);
	return (0);
}
