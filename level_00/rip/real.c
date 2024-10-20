#include <stdio.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}


void	correct_parathesis(char *str, int len, int i, int fix_l)
{
	int temp_i, count_l, count_r, break_flag;

	temp_i = i;
	count_l = 0;
	count_r = 0;
	break_flag = 0;

	if (i >= len)
		return;

	while (len > temp_i)
	{
		if (str[temp_i] == '(' && break_flag == 1)
			break;
		if (str[temp_i] == '(')
			count_l++;
		if (str[temp_i] == ')')
		{
			count_r++;
			break_flag = 1;
		}
		temp_i++;
	}
	if (count_l + fix_l > count_r)
	{
		// if (count_r > 0 && str[i + count_r] == '(')
		str[i] = ' ';
		correct_parathesis(str, len, i + 1, fix_l);
		if (count_r > 0)
		{
			str[i] = '(';
			fix_l++;
			correct_parathesis(str, len, i + 1, fix_l);
		} 
		else
			printf("l:%i, r:%i\n", count_l, count_r);
		return ;
	}
	// else if (count_l < count_r)
	// {
	// 	if (count_l > 0)
	// 		correct_parathesis(str, len, i + count_l);
	// 	// need to fix here
		// }
	else if (count_r == count_l && (str[temp_i] == '(' || str[temp_i] == ')'))
		return correct_parathesis(str, len, i + temp_i, 0);
	// else if (count_r == count_l)
		puts(str);
	return ;
}

void	cut_edge(char *str)
{
	int	i;
	int	len;

	i = 0;
	while (str[i] == ')')
	{
		str[i] = ' ';
		i++;
	}
	len = ft_strlen(str);
	while (str[len - 1] == '(')
	{
		str[len - 1] = ' ';
		len--;
	}
	correct_parathesis(str, len, 0, 0);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	cut_edge(av[1]);
	return (0);
}
