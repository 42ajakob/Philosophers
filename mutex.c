/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:16:30 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/12 13:16:53 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_printf(t_philo *philo, long time, int id, char *str)
{
	pthread_mutex_lock(&philo->mtx->mtx_printf);
	if (philo->dead == 1)
		return (pthread_mutex_unlock(&philo->mtx->mtx_printf), -1);
	printf("%ld %d %s\n", time, id, str);
	pthread_mutex_unlock(&philo->mtx->mtx_printf);
	return (0);
}

long	get_last_meal(t_philo *philo)
{
	long	last_meal;

	pthread_mutex_lock(&philo->mtx->mtx_last_meal);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->mtx->mtx_last_meal);
	return (last_meal);
}

int	get_n_eaten(t_philo *philo)
{
	int	n_eaten;

	pthread_mutex_lock(&philo->mtx->mtx_n_eaten);
	n_eaten = philo->n_eaten;
	pthread_mutex_unlock(&philo->mtx->mtx_n_eaten);
	return (n_eaten);
}

int	get_n_eat(t_philo *philo)
{
	int	n_eat;

	pthread_mutex_lock(&philo->mtx->mtx_n_eat);
	n_eat = philo->tbl->n_eat;
	pthread_mutex_unlock(&philo->mtx->mtx_n_eat);
	return (n_eat);
}
