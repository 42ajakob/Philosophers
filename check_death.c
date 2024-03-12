/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:57:37 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/12 14:21:39 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_eaten(t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (get_n_eat(philo) == -1)
		return (0);
	while (i < philo[0].tbl->n_philo)
	{
		if (get_n_eaten(&philo[i]) == get_n_eat(philo))
			j++;
		i++;
	}
	if (j == philo[0].tbl->n_philo)
		return (1);
	return (0);
}

static void	finish_philo(t_philo *philo, int n_philo)
{
	int	i;

	i = 0;
	while (i < n_philo)
	{
		philo[i].finish = 1;
		i++;
	}
}

static void	*exit_philo(t_philo *philo, int i)
{
	if (get_time() - get_last_meal(&philo[i]) >= philo[i].tbl->t_die
		&& check_eaten(philo) == 0)
	{
		if (get_n_eaten(&philo[i]) >= get_n_eat(philo)
			&& get_n_eat(philo) != -1)
			return (check_death(philo));
		pthread_mutex_lock(&philo[0].mtx->mtx_printf);
		printf("%ld %d died\n", get_time() - philo[i].s_t, philo[i].id + 1);
		finish_philo(philo, philo[0].tbl->n_philo);
		pthread_mutex_unlock(&philo[0].mtx->mtx_printf);
	}
	if (check_eaten(philo) == 1)
	{
		pthread_mutex_lock(&philo[0].mtx->mtx_printf);
		finish_philo(philo, philo[0].tbl->n_philo);
		pthread_mutex_unlock(&philo[0].mtx->mtx_printf);
	}
	return (NULL);
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	ft_usleep(philo[0].tbl->t_die - 10);
	while ((get_time() - get_last_meal(&philo[i]) < philo[i].tbl->t_die
			&& check_eaten(philo) == 0))
	{
		i++;
		if (i == philo[0].tbl->n_philo)
		{
			usleep(50);
			i = 0;
		}
	}
	exit_philo(philo, i);
	return (NULL);
}
