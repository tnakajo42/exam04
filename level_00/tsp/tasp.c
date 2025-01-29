#include <stdio.h>
#include <math.h>
#include <float.h>

float	calcilate_distance(double x0, double x1, double y0, double y1)
{
	double	dx;
	double	dy;

	dx = x1 - x0;
	dy = y1 - y0;
	
	return sqrt(dx * dx + dy * dy);
}

void	swap(int *a, int *b)
{
	int temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	find_shortest_path(int *perm, int start, int len, double x[], double y[], double *min_distance)
{
	int		i;
	double	total_distance;

	total_distance = 0;
	if (start == len - 1)
	{
		i = 0;
		while (i < len - 1)
		{
			total_distance += calcilate_distance(x[perm[i]], x[perm[i + 1]], y[perm[i]], y[perm[i + 1]]);
			i++;
		}
		if (total_distance < *min_distance)
			*min_distance = total_distance;
	}
	else
	{
		i = start;
		while (i < len - 1)
		{
			swap(&perm[start], &perm[i]);
			find_shortest_path(perm, start + 1, len, x, y, min_distance);
			swap(&perm[start], &perm[i]);
			i++;
		}
	}
}

int	main()
{
	int		len;
	int		perm[12];
	double	x[12];
	double	y[12];
	double	min_distance;

	len = 0;
	while (fscanf(stdin, "%lf, %lf", &x[len], &y[len]) == 2)
	{
		perm[len] = len;
		len++;
	}
	min_distance = DBL_MAX;
	find_shortest_path(perm, 1, len, x, y, &min_distance);
	printf("%.2lf\n", min_distance);
	return (0);
}