/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:37:20 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/10 19:27:25 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/* Mutex All That Shit included to Death checking AHHHHHH */
/* norminette */ /* Leaks and Hellgrind */

int	check_eaten(t_philo *philo)
{
	int i;
	int	j;

	i = 0;
	j = 1;
	if (philo[0].n_eat == -1)
		return (0);
	while (i < philo[0].tbl->n_philo)
	{
		if (philo[i].n_eaten == philo[i].n_eat)
			j++;
		i++;
	}
	if (j == philo[0].tbl->n_philo)
		return (1);
	return (0);
}

void	set_philo_dead(t_philo *philo, int n_philo)
{
	int i;

	i = 0;
	while (i < n_philo)
	{
		philo[i].dead = 1;
		i++;
	}
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	int 	i;

	philo = (t_philo *)arg;
	i = 0;
	ft_usleep(philo[0].tbl->t_die - 10);
	while ((get_time() < philo[i].last_meal + philo[i].tbl->t_die && philo[i].n_eaten != philo[i].n_eat && philo[i].n_eat != -1)
		|| (get_time() < philo[i].tbl->t_die + philo[i].last_meal && philo[i].n_eat == -1)) // mutex var
	{
		i++;
		if (i == philo[0].tbl->n_philo) // mutex var
		{
			ft_usleep(1);
			i = 0;
		}
		// if (check_eaten(philo) == 1) // Mutex var
		// 	return(printf("Ok!\n"), NULL);
	}
	if ((get_time() >= philo[i].last_meal + philo[i].tbl->t_die && philo[i].n_eaten != philo[i].n_eat && philo[i].n_eat != -1) 
		|| (get_time() >= philo[i].last_meal + philo[i].tbl->t_die && philo[i].n_eat == -1)) // mutex var
	{
		pthread_mutex_lock(philo[0].mutex_printf);
		printf("%ld %d died\n", get_time() - philo[i].sta_time, philo[i].id + 1); // mutex var
		pthread_mutex_unlock(philo[0].mutex_printf);
		set_philo_dead(philo, philo[0].tbl->n_philo); // mutex var
	}
	return (NULL);
}

int	simulation(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (philo->right_fork == NULL)
	{
		pthread_mutex_lock(philo->mutex_last_meal);
		philo->last_meal = get_time();
		pthread_mutex_unlock(philo->mutex_last_meal);
		pthread_mutex_lock(philo->mutex_printf);
		printf("%ld %d has taken a fork\n", get_time() - philo->sta_time, philo->id + 1);
		pthread_mutex_unlock(philo->mutex_printf);
		ft_usleep(philo->tbl->t_die + 10);
		return (-1);
	}
	pthread_mutex_lock(philo->right_fork);
	philo->cur_time = get_time();
	pthread_mutex_lock(philo->mutex_printf);
	if (philo->dead == 1)
		return (pthread_mutex_unlock(philo->mutex_printf), -1);
	printf("%ld %d has taken a fork\n", philo->cur_time - philo->sta_time, philo->id + 1);
	printf("%ld %d has taken a fork\n", philo->cur_time - philo->sta_time, philo->id + 1);
	printf("%ld %d is eating\n", philo->cur_time - philo->sta_time, philo->id + 1);
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
	// if (philo->dead == 1)
	// 	return (-1);
	pthread_mutex_lock(philo->mutex_printf);
	if (philo->dead == 1)
		return (pthread_mutex_unlock(philo->mutex_printf), -1);
	printf("%ld %d is sleeping\n", philo->cur_time - philo->sta_time, philo->id + 1);
	pthread_mutex_unlock(philo->mutex_printf);
	philo->cur_time = ft_usleep(philo->t_sleep);
	pthread_mutex_lock(philo->mutex_printf);
	if (philo->dead == 1)
		return (pthread_mutex_unlock(philo->mutex_printf), -1);
	printf("%ld %d is thinking\n", philo->cur_time - philo->sta_time, philo->id + 1);
	pthread_mutex_unlock(philo->mutex_printf);
	return (EXIT_SUCCESS);
}

void	*runtime(void *arg)
{
	t_philo *philo;
	int i;

	philo = (t_philo *)arg;
	i = 0;
	philo->sta_time = get_time();
	if (philo->id % 2 != 0)
		ft_usleep(philo->t_eat / 2);
	while (i != philo->n_eat && i != -1)
	{
		if (simulation(philo) == -1)
			return (NULL);
		i++;
	}
	return (NULL);
}

// void leaks()
// {
// 	system("leaks philo");
// }

int	main(int argc, char **argv)
{
	t_table	*tbl;
	t_philo *philo;

	// atexit(leaks);
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
