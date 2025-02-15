#include "argo.h"
#include <unistd.h> // for write()

int peek(FILE *stream) {
    int c = getc(stream);
	ungetc(c, stream);
    return c;
}

void unexpected(FILE *stream) {
    if (peek(stream) != EOF)
        printf("Unexpected token '%c'\n", peek(stream));
    else
		write(1, "Unexpected end of input\n", 24); // CHANGED because fflush(stdout) is not allowed
}

int accept(FILE *stream, char c) {
    if (peek(stream) == c) {
        (void) getc(stream);
        return 1;
    }
    return 0;
}

int expect(FILE *stream, char c) {
    if (accept(stream, c)) {
        return 1;
    }
    unexpected(stream);
    return 0;
}

//add your code from here below

int	parse_integer(int *dst, FILE *stream)
{
	if (fscanf(stream, "%d", dst) != 1)
	{
		unexpected(stream);
		return (-1);
	}
	return (1);
}

int parse_string(char **dst, FILE *stream)
{
	size_t size = 16;
	size_t len = 0;
	char *buffer = malloc(size);
	int c;

	if (!buffer)
		return (-1);
	if (!expect(stream, '"'))
	{
		free(buffer);
		return (-1);
	}
	while ((c = getc(stream)) != EOF)
	{
		if (c == '\\') {
			c = getc(stream);
			if (c != '\\' && c != '"')
			{
				free(buffer);
				unexpected(stream);
				return (-1);
			}
		}
		else if (c == '"')
		{
			buffer[len] = '\0';
			*dst = buffer;
			return (1);
		}
		if (len + 1 >= size)
		{
			size *= 2;
			char *new = realloc(buffer, size);
			if (!new) {
				free(buffer);
				return (-1);
			}
			buffer = new;
		}
		buffer[len++] = c;
	}
	free(buffer);
	unexpected(stream);
	return (-1);
}

int parse_map(json *dst, FILE *stream)
{
	dst->map.size = 0;
    dst->map.data = NULL;
	char *key;
	
	// Ensure opening brace is consumed
	if (!expect(stream, '{'))
		return -1;
    while (peek(stream) != '}')
	{
		if (dst->map.size > 0 && !expect(stream, ','))
			return (-1);
		// Parse key
		key = NULL;
		if (parse_string(&key, stream) == -1)
			return (-1);
		// Parse colon (:)
		if (!expect(stream, ':'))
		{
			free(key);
			return (-1);
		}
        // Parse value
        json value;
        if (argo(&value, stream) == -1)
		{
            free(key);
            return (-1);
        }
        // Add key-value pair to map
        dst->map.data = realloc(dst->map.data, sizeof(pair) * (dst->map.size + 1));
        if (!dst->map.data)
		{
            free(key);
            free_json(value);
			free(dst->map.data);
			return (-1);
		}
		dst->map.data[dst->map.size].key = key;
		dst->map.data[dst->map.size].value = value;
		dst->map.size++;
    }
    // Consume the closing brace
    if (!expect(stream, '}'))
        return (-1);
    return (1);
}

int argo(json* dst, FILE* stream)
{
	int		c = peek(stream);

	if (isdigit(c) || c == '-')
	{
		dst->type = INTEGER;
		return parse_integer(&dst->integer, stream);
	}
	else if (c == '"')
	{
		dst->type = STRING;
		return parse_string(&dst->string, stream);
	}
	else if (c == '{')
	{
		dst->type = MAP;
		return parse_map(dst, stream);
	}
	unexpected(stream);
	return (-1);
}

