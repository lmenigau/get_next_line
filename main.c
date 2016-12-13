/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenigau <lmenigau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/10 11:07:03 by lmenigau          #+#    #+#             */
/*   Updated: 2016/12/13 14:55:33 by lmenigau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int		main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		ret;

	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	else {
		fd = 0;
	}
	while ((ret = get_next_line(fd, &line)))
	{
		printf("%d	|%s\n", ret, line);
		free(line);
	}
	return (0);
}
