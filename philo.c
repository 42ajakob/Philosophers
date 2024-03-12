/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:37:20 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/12 15:12:12 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	one_philo(t_philo *p)
{
	pthread_mutex_lock(&p->mtx->mtx_last_meal);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->mtx->mtx_last_meal);
	get_printf(p, get_time() - p->s_t, p->id + 1, "has taken a fork");
	pthread_mutex_unlock(p->left_fork);
	ft_usleep(p->t_die + 10);
	return (-1);
}

int	simulation(t_philo *p)
{
	pthread_mutex_lock(p->left_fork);
	if (p->right_fork == NULL)
		return (one_philo(p));
	pthread_mutex_lock(p->right_fork);
	p->c_t = get_time();
	get_printf(p, p->c_t - p->s_t, p->id + 1, "has taken a fork");
	get_printf(p, p->c_t - p->s_t, p->id + 1, "has taken a fork");
	get_printf(p, p->c_t - p->s_t, p->id + 1, "is eating");
	pthread_mutex_lock(&p->mtx->mtx_last_meal);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->mtx->mtx_last_meal);
	pthread_mutex_lock(&p->mtx->mtx_n_eaten);
	p->n_eaten++;
	pthread_mutex_unlock(&p->mtx->mtx_n_eaten);
	p->c_t = ft_usleep(p->t_eat);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	if (get_printf(p, p->c_t - p->s_t, p->id + 1, "is sleeping") == -1)
		return (-1);
	p->c_t = ft_usleep(p->t_sleep);
	if (get_printf(p, p->c_t - p->s_t, p->id + 1, "is thinking") == -1)
		return (-1);
	return (EXIT_SUCCESS);
}

void	*runtime(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	philo->s_t = get_time();
	if (philo->id % 2 != 0)
		ft_usleep(philo->t_eat / 2);
	while (i != get_n_eat(philo) && i != -1)
	{
		if (simulation(philo) == -1)
			return (NULL);
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_table	*tbl;
	t_philo	*philo;

	if (valid_args(argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	tbl = init_table(argc, argv);
	if (!tbl)
		return (EXIT_FAILURE);
	philo = init_philo(tbl);
	if (!philo)
		return (free_all(tbl, philo), EXIT_FAILURE);
	if (init_thread(tbl, philo) == -1)
		return (free_all(tbl, philo), EXIT_FAILURE);
	free_all(tbl, philo);
	return (EXIT_SUCCESS);
}
