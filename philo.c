/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:37:20 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/09 16:17:47 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/* Clean Code */
/* Death + Mutex Death + Mutex t_die */
/* 1 Philo only */
/* norminette */

int	simulation(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	philo->cur_time = get_time();
	pthread_mutex_lock(philo->mutex_printf);
	printf("%ld %d has taken a fork\n", philo->cur_time - philo->sta_time, philo->id);
	printf("%ld %d has taken a fork\n", philo->cur_time - philo->sta_time, philo->id);
	printf("%ld %d is eating\n", philo->cur_time - philo->sta_time, philo->id);
	pthread_mutex_unlock(philo->mutex_printf);
	pthread_mutex_lock(philo->mutex_last_meal);
	philo->last_meal = philo->cur_time;
	pthread_mutex_unlock(philo->mutex_last_meal);
	pthread_mutex_lock(philo->mutex_eaten);
	philo->n_eaten++;
	pthread_mutex_unlock(philo->mutex_eaten);
	philo->cur_time = ft_usleep(philo->t_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(philo->mutex_printf);
	printf("%ld %d is sleeping\n", philo->cur_time - philo->sta_time, philo->id);
	pthread_mutex_unlock(philo->mutex_printf);
	philo->cur_time = ft_usleep(philo->t_sleep);
	pthread_mutex_lock(philo->mutex_printf);
	printf("%ld %d is thinking\n", philo->cur_time - philo->sta_time, philo->id);
	pthread_mutex_unlock(philo->mutex_printf);
	return (0);
}

void	*runtime(void *arg)
{
	t_philo *philo;
	philo = (t_philo *)arg;
	int i;

	i = 0;
	philo->sta_time = get_time();
	if (philo->id % 2 != 0)
		ft_usleep(philo->t_eat / 2);
	while (i != philo->n_eat && i != -1)
	{
		simulation(philo);
		i++;
	}
	return (NULL);
}

void leaks()
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_table	*tbl;
	t_philo *philo;

	atexit(leaks);
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
