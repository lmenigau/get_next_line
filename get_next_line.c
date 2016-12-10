/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenigau <lmenigau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:05:57 by lmenigau          #+#    #+#             */
/*   Updated: 2016/12/10 16:10:29 by lmenigau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file	*find_or_create_struct(t_list *files, int fd)
{
	t_file	*file;
	t_list	*prevfile;
	t_file	*newfile;

	prevfile = files->content;
	while (files != NULL && prevfile)
	{
		file = files->content;
		if (file->fd == fd)
			return (file);
		files = files->next;
		prevfile = files;
	}
	if ((newfile = malloc(sizeof (t_file))) == NULL)
		return (NULL);
	newfile->fd = fd;
	newfile->rest = NULL;
	newfile->size = 0;
	if (prevfile)
		prevfile->next->content = newfile;
	else
		prevfile->content = newfile;
	return (newfile);
}

int		extract_line(t_file *file, char **line, char *buff, size_t size)
{
	char	*nl;
	if ((nl = ft_memchr(buff, '\n', size)))
	{
		*line = ft_memjoin(NULL, buff, 0, buff - nl);
		*nl = '\0';
		file->size = size - (file->rest - nl);
		free(file->rest);
		file->rest = ft_memjoin(nl, NULL, file->size, 0);
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
	char	*nl;

	buff = malloc(BUFF_SIZE);
	nl = NULL;
	if (file->size != 0)
	{
		if (extract_line(file, line, buff, file->size) == -1)
			return (-1);
	}
	while ((byte_read = read(file->fd, buff, BUFF_SIZE)) > 0)
	{
		if (extract_line(file, line, buff, byte_read) == -1)
				return (-1);
		else
			ft_memjoin(file->rest, buff, file->size, byte_read);
	}
	free(buff);
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	t_file			*file;
	static t_list	*files = NULL;

	if (files == NULL && (files = malloc(sizeof *files)) != NULL)
		return (-1);
	file = find_or_create_struct(files, fd);
	if (manage_file(file, line) == 1)
		return (1);
	return (0);
}
