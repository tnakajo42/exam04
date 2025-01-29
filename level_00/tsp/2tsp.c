#include <stdio.h>
#include <math.h>
#include <float.h>

float	dis(double x0, double y0,double x1, double y1)
{
	float	dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	return (sqrt(dx * dx + dy * dy));
}

void	swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void	perm(int *p, int start, int end, double x[], double y[], double *min)
{
	double	tot_d = 0;
	int		i = 0;
	if (start == end)
	{
		while (i < end)
		{
			tot_d += dis(x[p[i]], y[p[i]], x[p[i + 1]], y[p[i + 1]]);
			i++;
		}
		tot_d += dis(x[p[end]], y[p[end]], x[p[0]], y[p[0]]);
		if (tot_d < *min)
			*min = tot_d;
	}
	i = start;
	while (i < end)
	{
		swap(&p[start], &p[i]);
		perm(p, start + 1, end, x, y, min);
		swap(&p[start], &p[i]);
		i++;
	}
}

int main (void)
{
	double	x[11], y[11];
	double	min;
	int		p[11];
	int		len;
	len = 0;
	while (fscanf(stdin, "%lf, %lf", &x[len], &y[len]) == 2)
	{
		p[len] = len;
		len++;
	}
	min = DBL_MAX;
	perm(p, 0, len - 1, x, y, &min);
	printf("%.2lf\n", min);
	return 0;
}