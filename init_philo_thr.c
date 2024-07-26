/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_thr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:41:32 by ajakob            #+#    #+#             */
/*   Updated: 2024/07/26 17:41:03 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	sort_mutex_pointers(t_table *tbl, t_philo *philo, int i)
{
	philo->left_fork = &tbl->mtx->forks[i];
	if (i + 1 < tbl->n_philo)
		philo->right_fork = &tbl->mtx->forks[i + 1];
	else if (i + 1 == tbl->n_philo && tbl->n_philo > 1)
		philo->right_fork = &tbl->mtx->forks[0];
	else
		philo->right_fork = NULL;
	philo->mtx = tbl->mtx;
}

t_philo	*init_philo(t_table *tbl)
{
	t_philo	*philo;
	int		i;

	philo = ft_calloc(sizeof(t_philo) * tbl->n_philo);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < tbl->n_philo)
	{
		philo[i].id = i;
		philo[i].t_die = tbl->t_die;
		philo[i].t_eat = tbl->t_eat;
		philo[i].t_sleep = tbl->t_sleep;
		philo[i].n_eaten = 0;
		philo[i].finish = 0;
		philo[i].tbl = tbl;
		philo[i].last_meal = get_time();
		sort_mutex_pointers(tbl, &philo[i], i);
		i++;
	}
	return (philo);
}

int	ctr_join_thr(t_table *tbl, pthread_t *thr, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < tbl->n_philo)
	{
		if (pthread_create(&thr[i], NULL, &runtime, (void *)&philo[i]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&thr[i], NULL, &check_death, (void *)philo) != 0)
		return (-1);
	i = 0;
	while (i < tbl->n_philo + 1)
	{
		if (pthread_join(thr[i], NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}

int	init_thread(t_table *tbl, t_philo *philo)
{
	pthread_t	*thr;

	thr = ft_calloc(sizeof(pthread_t) * tbl->n_philo + 1);
	if (!thr)
		return (-1);
	if (ctr_join_thr(tbl, thr, philo) == -1)
		return (free(thr), -1);
	free(thr);
	return (0);
}
