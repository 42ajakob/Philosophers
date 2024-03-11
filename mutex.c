# include "philo.h"

int	get_printf(t_philo *philo, long time, int id, char *str)
{

	pthread_mutex_lock(philo->mutex_printf);
	if (philo->dead == 1)
		return (pthread_mutex_unlock(philo->mutex_printf), -1);
	printf("%ld %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->mutex_printf);
	return (0);
}

// void	get_n_eat(t_philo *philo, int n_eat)
// {
// 	pthread_mutex_lock(philo->mutex_n_eat);
// 	philo->n_eat = n_eat;
// 	pthread_mutex_unlock(philo->mutex_n_eat);
// }

// void	get_n_eaten(t_philo *philo, int n_eaten)
// {
// 	pthread_mutex_lock(philo->mutex_eaten);
// 	philo->n_eaten = n_eaten;
// 	pthread_mutex_unlock(philo->mutex_eaten);
// }

// void	get_last_meal(t_philo *philo, long last_meal)
// {
// 	pthread_mutex_lock(philo->mutex_last_meal);
// 	philo->last_meal = last_meal;
// 	pthread_mutex_unlock(philo->mutex_last_meal);
// }

// long	get_t_die(t_philo *philo, long t_die)
// {
// 	pthread_mutex_lock(philo->mutex_t_die);
// 	philo->tbl->t_die = t_die;
// 	pthread_mutex_unlock(philo->mutex_t_die);
// 	return (philo->tbl->t_die);
// }
