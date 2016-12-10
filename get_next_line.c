/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenigau <lmenigau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 14:05:57 by lmenigau          #+#    #+#             */
/*   Updated: 2016/12/10 14:50:17 by lmenigau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file	*find_or_create_struct(t_list *files, int fd)
{
	t_file	*file;
	t_file	newfile;
	t_list	*prevfile;

	while (files != NULL)
	{
		file = files->content;
		if (file->fd == fd)
			return (file);
		files = files->next;
		prevfile = files;
	}
	newfile.fd = fd;
	newfile.rest = NULL;
	newfile.size = 0;
	prevfile->next = ft_lstnew(&newfile, sizeof (newfile));
	return (prevfile->next->content);
}

int		extract_line(t_file *file, char **line, char *buff, size_t size)
{
	char	*nl;
	if ((nl = ft_memchr(buff, '\n', size)))
	{
			*line = ft_memjoin(NULL, buff, 0, buff - nl);
			file->rest = ft_memjoin(nl, file->size - (file->rest - nl));
			return (1);
	}
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
}

int		get_next_line(const int fd, char **line)
{
	t_file			*file;
	static t_list	*files = NULL;

	if (files == NULL && (files = malloc(sizeof *files)) != NULL)
		return (-1);
	file = find_or_create_struct(files, fd);
	manage_file(file, line);
	return (0);
}
