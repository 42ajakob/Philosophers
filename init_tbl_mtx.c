/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tbl_mtx.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 13:17:06 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/12 13:42:49 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_mtx *mtx, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&mtx->forks[j]);
		j++;
	}
}

int	init_l_meal_n_eaten_n_eat(t_mtx *mtx, int i)
{
	if (pthread_mutex_init(&mtx->mtx_last_meal, NULL) != 0)
	{
		pthread_mutex_destroy(&mtx->mtx_printf);
		return (destroy_forks(mtx, i), -1);
	}
	if (pthread_mutex_init(&mtx->mtx_n_eaten, NULL) != 0)
	{
		pthread_mutex_destroy(&mtx->mtx_printf);
		pthread_mutex_destroy(&mtx->mtx_last_meal);
		return (destroy_forks(mtx, i), -1);
	}
	if (pthread_mutex_init(&mtx->mtx_n_eat, NULL) != 0)
	{
		pthread_mutex_destroy(&mtx->mtx_printf);
		pthread_mutex_destroy(&mtx->mtx_last_meal);
		pthread_mutex_destroy(&mtx->mtx_n_eaten);
		return (destroy_forks(mtx, i), -1);
	}
	return (0);
}

int	init_mtx(t_table *tbl)
{
	t_mtx	*mtx;
	int		i;

	i = 0;
	mtx = ft_calloc(sizeof(t_mtx));
	if (!mtx)
		return (-1);
	mtx->forks = ft_calloc(sizeof(pthread_mutex_t) * tbl->n_philo + 1);
	if (!mtx->forks)
		return (free(mtx), mtx->forks = NULL, -1);
	while (i < tbl->n_philo)
	{
		if (pthread_mutex_init(&mtx->forks[i], NULL) != 0)
			return (destroy_forks(mtx, i), -1);
		i++;
	}
	if (pthread_mutex_init(&mtx->mtx_printf, NULL) != 0)
		return (destroy_forks(mtx, i), -1);
	if (init_l_meal_n_eaten_n_eat(mtx, i) == -1)
		return (-1);
	tbl->mtx = mtx;
	return (0);
}

t_table	*init_table(int argc, char **argv)
{
	t_table	*tbl;

	tbl = ft_calloc(sizeof(t_table));
	if (!tbl)
		return (NULL);
	tbl->n_philo = ft_atoi(argv[1]);
	tbl->t_die = ft_atoi(argv[2]);
	tbl->t_eat = ft_atoi(argv[3]);
	tbl->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		tbl->n_eat = ft_atoi(argv[5]);
	else
		tbl->n_eat = -1;
	if (init_mtx(tbl) == -1)
	{
		if (tbl->mtx->forks)
			free(tbl->mtx->forks);
		if (tbl->mtx)
			free(tbl->mtx);
		return (free(tbl), NULL);
	}
	return (tbl);
}
