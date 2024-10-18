#include <stdio.h>
#include <unistd.h>

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void correct_parathesis(char *str)
{
	puts(str); // Have to fix here later
}

void cut_edge(char *str)
{
	int i;

	i = 0;
	while (str[i] == ')')
	{
		str[i] = ' ';
		i++;
	}
	i = ft_strlen(str);
	while (str[i - 1] == '(')
	{
		str[i - 1] = ' ';
		i--;
	}
	correct_parathesis(str);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	cut_edge(av[1]);
	return (0);
}
