# include "philo.h"

void	destroy_forks(t_table *tbl, int i)
{
	int j;
	
	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&tbl->forks[j]);
		j++;
	}
}

t_table	*init_table(int argc, char **argv)
{
	t_table	*tbl;
	int i;

	i = 0;
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
	tbl->forks = ft_calloc(sizeof(pthread_mutex_t) * tbl->n_philo + 1);
	if (!tbl->forks)
		return (free(tbl), NULL);
	while (i < tbl->n_philo)
	{
		if (pthread_mutex_init(&tbl->forks[i], NULL) != 0)
		{
			destroy_forks(tbl, i);
			return (free(tbl), NULL);
		}
		i++;
	}
	if (pthread_mutex_init(&tbl->mutex_printf, NULL) != 0)
	{
		destroy_forks(tbl, i);
		return (free(tbl), NULL);
	}
	if (pthread_mutex_init(&tbl->mutex_eaten, NULL) != 0)
	{
		destroy_forks(tbl, i);
		pthread_mutex_destroy(&tbl->mutex_printf);
		return (free(tbl), NULL);
	}
	if (pthread_mutex_init(&tbl->mutex_last_meal, NULL) != 0)
	{
		destroy_forks(tbl, i);
		pthread_mutex_destroy(&tbl->mutex_printf);
		pthread_mutex_destroy(&tbl->mutex_eaten);
		return (free(tbl), NULL);
	}
	return (tbl);
}

t_philo	*init_philo(t_table *tbl)
{
	t_philo *philo;
	int	i;

	philo = ft_calloc(sizeof(t_philo) * tbl->n_philo);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < tbl->n_philo)
	{
		philo[i].id = i + 1;
		philo[i].t_eat = tbl->t_eat;
		philo[i].t_sleep = tbl->t_sleep;
		philo[i].n_eat = tbl->n_eat;
		philo[i].n_eaten = 0;
		philo[i].dead = 0;
		philo[i].left_fork = &tbl->forks[i];
		if (i + 1 < tbl->n_philo) // if init_mutex failed
			philo[i].right_fork = &tbl->forks[i + 1];
		else if (i + 1 == tbl->n_philo && tbl->n_philo > 1)
			philo[i].right_fork = &tbl->forks[0];
		else
			philo[i].right_fork = NULL;
		init_mutex(tbl, &philo[i]);
		i++;
	}
	return (philo);
}

void	init_mutex(t_table *tbl, t_philo *philo) // LATER Death
{
	philo->mutex_printf = &tbl->mutex_printf;
	philo->mutex_eaten = &tbl->mutex_eaten;
	philo->mutex_last_meal = &tbl->mutex_last_meal;
}

int init_thread(t_table *tbl, t_philo *philo)
{
	pthread_t *thread;
	int *j;
	int i;

	i = 0;
	thread = ft_calloc(sizeof(pthread_t) * tbl->n_philo); // plus 1 for checking deaths
	if (!thread)
		return (-1);
	j = ft_calloc(sizeof(int) * tbl->n_philo); // mhhh
	if (!j)
		return (free(thread), -1);
	while (i < tbl->n_philo)
	{
		j[i] = i;
		i++;
	}
	i = 0;
	while (i < tbl->n_philo)
	{
		if (pthread_create(&thread[i], NULL, &runtime, (void *)&philo[j[i]]) != 0)
			return (-1);
		i++;
	}
	i = 0;
	while (i < tbl->n_philo)
	{
		if (pthread_join(thread[i], NULL) != 0)
			return (-1);
		i++;
	}
	free(j);
	free(thread);
	return (0);
}
