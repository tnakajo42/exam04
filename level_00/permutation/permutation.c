#include <stdio.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_swap(char *x, char *y)
{
	char	temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

void	ft_alphabetical(char *str, int len, int start)
{
	int	i;
	int	j;

	i = start;
	while (i < len - 1)
	{
		j = start;
		while (j < len - 1)
		{
			if (str[j] > str[j + 1])
				ft_swap(&str[j], &str[j + 1]);
			j++;
		}
		i++;
	}
}

void	ft_permute(char *str, int start, int end)
{
	int	i;

	i = start;
	if (str && start == end)
		puts(str);
	else
	{
		while (i <= end)
		{
			ft_alphabetical(str, end + 1, start);
			ft_swap(&str[start], &str[i]);
			ft_permute(str, start + 1, end);
			ft_swap(&str[start], &str[i]);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	int	i;

	if (ac != 2)
		return (1);
	i = ft_strlen(av[1]);
	ft_permute(av[1], 0, i - 1);
	return (0);
}
