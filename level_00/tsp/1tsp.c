#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


float	distance(float x0, float x1, float y0, float y1)
{
	float dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	return (sqrt(dx * dx + dy * dy));
}

void	swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void	perm_ft(int *perm, int start, int end, double x[], double y[], double *min)
{
	// int  count = 0;
	float	td = 0;
	int i = 0;
	if (start == end)
	{
		while (i < end)
		{
			td += distance(x[perm[i]], x[perm[i + 1]], y[perm[i]], y[perm[i + 1]]);
			i++;
		}
		td += distance(x[perm[end]], x[perm[0]], y[perm[end]], y[perm[0]]);
		if (*min > td)
			*min = td;
	}
	i = start;
	while (i < end)
	{
		swap(&perm[start], &perm[i]);
		perm_ft(perm, start + 1, end, x, y, min);
		swap(&perm[start], &perm[i]);
		i++;
	}
	
}

int	main(void)
{
	double x[11], y[11];
	int perm[11];
	double	min;
	int	i = 0;
	while (fscanf(stdin, "%lf, %lf", &x[i], &y[i]) == 2)
	{
		perm[i] = i;
		i++;
	}
	int len;
	len = i;
	min = DBL_MAX;
	perm_ft(perm, 0, len - 1, x, y, &min);
	printf("after: %.2lf\n", min);
	return 0;
}