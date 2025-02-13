/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnakajo <tnakajo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:03:20 by cadenegr          #+#    #+#             */
/*   Updated: 2025/02/13 18:21:10 by tnakajo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int	peek(FILE *stream)
{
	int c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("Unexpected token: %c\n", peek(stream));
	else
		printf("Unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

int	parse_integer(int *dst, FILE *stream)
{
	int neg = 0;
	int c = peek(stream);
	if (c == '-')
	{
		neg++;
		accept(stream, c);
	}
	c = peek(stream);
	if (!isdigit(c))
	{
		unexpected(stream);
		return -1;
	}
	fscanf(stream, "%d", dst);
	if (neg)
		*dst *= -1;
	return 1;
}

int	parse_string(char **dst, FILE *stream)
{
	if (!accept(stream, '"'))
	{
		unexpected(stream);
		return -1;
	}
	size_t cap = 16, len = 0;
	*dst = malloc(cap);
	if (!*dst)
		return -1;
	int c;
	while ((c = getc(stream)) != EOF)
	{
		if (c == '"')
			break;
		if (c == '\\')
		{
			int esc = getc(stream);
			if (esc == '"' || esc == '\\')
				c = esc;
			else
			{
				unexpected(stream);
				return -1;
			}
		}
		if (len + 1 >= cap)
		{
			cap *= 2;
			char *new = realloc(*dst, cap);
			if (!new)
			{
				return -1;
			}
			*dst = new;
		}
		(*dst)[len] = c;
		len++;
	}
	(*dst)[len] = '\0';
	// printf("------->%c\n", peek(stream));
	if (c != '"')
	{
		unexpected(stream);
		return -1;
	}
	return 1;
}


int	parse_map(json *j, FILE *stream)
{
	if (!accept(stream, '{'))
		return -1;
	size_t cap = 4, size = 0;
	j->type = MAP;
	j->map.data = malloc(sizeof(pair) * cap);
	if (!j->map.data)
		return -1;
	while (!accept(stream, '}'))
	{
		if (size + 1 > cap)
		{
			cap *= 2;
			j->map.data = realloc(j->map.data, sizeof(pair) * cap);
			if (!j->map.data)
				return -1;
		}
		pair	*p = &j->map.data[size];
		if (parse_string(&p->key, stream) == -1 || !expect(stream, ':'))
		{
			free_json(*j);
			return -1;
		}
		if (argo(&p->value, stream) == -1)
		{
			free_json(*j);
			return -1;
		}
		if (!accept(stream, ',') && peek(stream) != '}')
		{
			free_json(*j);
			unexpected(stream);
			return -1;
		}
		size++;
	}
	j->map.size = size;
	return 1;
}

int	argo(json *dst, FILE *stream)
{
	static int	open = 0;
	static int	close = 0;
	int			c = peek(stream);
	if (c == '{')
	{
		open++;
		int	res = 0;
		if ((res = parse_map(dst, stream)) == -1)
			return -1;
		close += res;
	}
	else if(c == '"')
	{
		dst->type = STRING;
		int	res_str = 0;
		res_str = parse_string(&dst->string, stream);
		if (res_str == -1)
			free_json(*dst);
		return res_str;
	}
	else if(isdigit(c) || c == '-')
	{
		dst->type = INTEGER;
		return parse_integer(&dst->integer, stream);
	}
	else
	{
		unexpected(stream);
		return -1;
	}

	c = peek(stream);
	if (open == close && c != EOF)
	{
		unexpected(stream);
		free_json(*dst);
		return -1;
	}
	return 1;
}
