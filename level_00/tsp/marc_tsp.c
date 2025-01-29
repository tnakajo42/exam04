#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef struct
{
	float	x;
	float	y;
}	City;

float distance(float x0, float y0, float x1, float y1)
{
	float diff[2] = {
	x1 - x0,
	y1 - y0
	};
	return sqrtf(diff[0] * diff[0] + diff[1] * diff[1]);
}

void	swap(int *a, int *b)
{
	int	tmp = *a;
	*a = *b;
	*b = tmp;
}

float	calculate_path_len(City *cities, int *path, int n)
{
	float totalDist = 0;
	int i = 0;

	while (i < n - 1)
	{
		totalDist += distance(cities[path[i]].x, cities[path[i]].y, cities[path[i + 1]].x, cities[path[i + 1]].y);
		i++;
	}
	totalDist += distance(cities[path[n - 1]].x, cities[path[n - 1]].y, cities[path[0]].x, cities[path[0]].y);
	return (totalDist);
}

void	permute(City *cities, int *path, int start, int end, float *minDist)
{
	float	currDist;
	int		i = start;

	if (start == end)
	{
		currDist = calculate_path_len(cities, path, end + 1);
		if (currDist < *minDist)
			*minDist = currDist;
	}
	else
	{
		while (i <= end)
		{
			swap(&path[start], &path[i]);
			permute(cities, path, start + 1, end, minDist);
			swap(&path[start], &path[i]);
			i++;
		}
	}
}

int main()
{
	City	cities[11];
	int		path[11];
	int		n = 0;
	float	minDist = FLT_MAX;

	while(fscanf(stdin, "%f, %f\n", &cities[n].x, &cities[n].y) == 2)
	{
		path[n] = n;
		n++;
	}
	permute(cities, path, 0, n - 1, &minDist);
	fprintf(stdout, "%.2f\n", minDist);
	return (0);
}