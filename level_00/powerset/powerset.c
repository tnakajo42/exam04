/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnakajo <tnakajo@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:09:35 by tnakajo           #+#    #+#             */
/*   Updated: 2024/10/25 17:33:44 by tnakajo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sumsub(int *arr, int n, int mask)
{
    int total = 0;
    for (int i = 0; i < n; i++)
    {
        if (mask & 1 << i)
            total += arr[i];
    }
    return total;
}
void printsub(int *arr, int n, int mask)
{
    int first = 0;
    for (int i = 0; i < n; i++)
    {
        if (mask & 1 << i)
        {
            if (!first)
            {
                fprintf(stdout, "%d", arr[i]);
                first = 1;
            }
            else
                fprintf(stdout, " %d", arr[i]);
        }
    }
    fprintf(stdout, "\n");
}


void    permute(int *arr, int n, int base)
{
    int total_subsets = 1 << n;
    int another = n * n;
    printf("---->%d and %d\n", total_subsets, another);
    int mask = 0;
    while (mask < total_subsets)
    {
        if (sumsub(arr, n, mask) == base)
        {
            printsub(arr, n, mask);
        }
        mask++;
    }

}

int main(int ac, char **av)
{
    if (ac < 3)
        return 1;
    int n = ac - 2;
    int base = atoi(av[1]);
    if (base < 0)
        return 1;
    int *arr = malloc(sizeof(int) * n);
    int i = 0;
    while (i < n)
    {
        arr[i] = atoi(av[i + 2]);
        i++;
    }
    permute(arr, n, base);
    return 0;
}