/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:04:44 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/11 15:00:46 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	free_all(t_table *tbl, t_philo *philo)
{
	int	i;

	i = 0;
	if (tbl->mtx->forks)
	{
		while (i < tbl->n_philo)
		{
			pthread_mutex_destroy(&tbl->mtx->forks[i]);
			i++;
		}
		free(tbl->mtx->forks);
	}
	pthread_mutex_destroy(&tbl->mtx->mtx_printf);
	pthread_mutex_destroy(&tbl->mtx->mtx_eaten);
	pthread_mutex_destroy(&tbl->mtx->mtx_last_meal);
	if (tbl->mtx)
		free(tbl->mtx);
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

long	get_time(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); // mil_sec since 1970
}

long ft_usleep(long mil_sec)
{
	long start;
	long wait;

	start = get_time();
	wait = start + mil_sec;
	while (get_time() < wait) // Include Death Check
		usleep(50);
	return (get_time());
}
