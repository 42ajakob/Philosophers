/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:04:44 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/09 15:46:22 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	free_all(t_table *tbl, t_philo *philo)
{
	int	i;

	i = 0;
	if (tbl->forks)
	{
		while (i < tbl->n_philo)
		{
			pthread_mutex_destroy(&tbl->forks[i]);
			i++;
		}
		free(tbl->forks);
	}
	pthread_mutex_destroy(&tbl->mutex_printf);
	pthread_mutex_destroy(&tbl->mutex_eaten);
	pthread_mutex_destroy(&tbl->mutex_last_meal);
	if (tbl)
		free(tbl);
	if (philo)
		free(philo);
}

void	*ft_calloc(size_t count)
{
	void	*ptr;

	ptr = malloc(count);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, count);
	return (ptr);
}

int	ft_atoi(const char *str)
{
	int	res;

	res = 0;
	while (*str >= 48 && *str <= 57)
	{
		res = res * 10 + (*str - 48);
		str++;
	}
	return (res);
}
