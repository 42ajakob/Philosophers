/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:37:20 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/11 15:44:00 by ajakob           ###   ########.fr       */
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

/*
philo->n_eat
*/
void	*check_death(void *arg)
{
	t_philo	*philo;
	int 	i;

	philo = (t_philo *)arg;
	i = 0;
	ft_usleep(philo[0].tbl->t_die - 10);
	while ((get_time() < get_last_meal(philo) + philo[i].tbl->t_die && check_eaten(philo) == 0))
	{
		
		i++;
		if (i == philo[0].tbl->n_philo)
		{
			ft_usleep(1);
			i = 0;
		}
	}
	if (get_time() >= get_last_meal(philo) + philo[i].tbl->t_die && check_eaten(philo) == 0)
	{
		if (get_n_eaten(&philo[i]) >= get_n_eat(philo) && get_n_eat(philo) != -1) // mtx
			return (check_death(philo));
		pthread_mutex_lock(&philo[0].mtx->mtx_printf);
		printf("%ld %d died\n", get_time() - philo[i].sta_time, philo[i].id + 1);
		set_philo_dead(philo, philo[0].tbl->n_philo);
		pthread_mutex_unlock(&philo[0].mtx->mtx_printf);
	}
	return (NULL);
}

int	one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->mtx->mtx_last_meal);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->mtx->mtx_last_meal);
	get_printf(philo, get_time() - philo->sta_time, philo->id + 1, "has taken a fork");
	ft_usleep(philo->t_die + 10);
	return (-1);
}

int	simulation(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (philo->right_fork == NULL)
		return (one_philo(philo));
	pthread_mutex_lock(philo->right_fork);
	philo->cur_time = get_time();
	get_printf(philo, philo->cur_time - philo->sta_time, philo->id + 1, "has taken a fork");
	get_printf(philo, philo->cur_time - philo->sta_time, philo->id + 1, "has taken a fork");
	get_printf(philo, philo->cur_time - philo->sta_time, philo->id + 1, "is eating");
	pthread_mutex_lock(&philo->mtx->mtx_last_meal);
	philo->last_meal = philo->cur_time;
	pthread_mutex_unlock(&philo->mtx->mtx_last_meal);
	pthread_mutex_lock(&philo->mtx->mtx_n_eaten);
	philo->n_eaten++;
	pthread_mutex_unlock(&philo->mtx->mtx_n_eaten);
	philo->cur_time = ft_usleep(philo->t_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (get_printf(philo, philo->cur_time - philo->sta_time, philo->id + 1, "is sleeping") == -1)
		return (-1);
	philo->cur_time = ft_usleep(philo->t_sleep);
	if (get_printf(philo, philo->cur_time - philo->sta_time, philo->id + 1, "is thinking") == -1)
		return (-1);
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
	while (i != get_n_eat(philo) && i != -1)
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
