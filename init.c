# include "philo.h"

void	destroy_forks(t_mtx *mtx, int i)
{
	int j;
	
	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&mtx->forks[j]);
		j++;
	}
}

int	init_mtx(t_table *tbl)
{
	t_mtx	*mtx;
	int i;

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
	if (pthread_mutex_init(&mtx->mtx_eaten, NULL) != 0)
		return (destroy_forks(mtx, i), pthread_mutex_destroy(&mtx->mtx_printf), -1);
	if (pthread_mutex_init(&mtx->mtx_last_meal, NULL) != 0)
	{
		pthread_mutex_destroy(&mtx->mtx_printf);
		pthread_mutex_destroy(&mtx->mtx_eaten);
		return (destroy_forks(mtx, i), -1);
	}
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

void	sort_mutex_pointers(t_table *tbl, t_philo *philo, int i) // LATER Death
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
	t_philo *philo;
	int	i;

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
		philo[i].n_eat = tbl->n_eat;
		philo[i].n_eaten = 0;
		philo[i].dead = 0;
		philo[i].tbl = tbl;
		philo[i].last_meal = get_time();
		sort_mutex_pointers(tbl, &philo[i], i);
		i++;
	}
	return (philo);
}

int	create_and_join_threads(t_table *tbl, pthread_t *thread, t_philo *philo, int *j)
{
	int i;

	i = 0;
	while (i < tbl->n_philo)
	{
		if (pthread_create(&thread[i], NULL, &runtime, (void *)&philo[j[i]]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&thread[i], NULL, &check_death, (void *)philo) != 0)
		return (-1);
	i = 0;
	while (i < tbl->n_philo + 1)
	{
		if (pthread_join(thread[i], NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}

int init_thread(t_table *tbl, t_philo *philo)
{
	pthread_t *thread;
	int *j;
	int i;

	i = 0;
	thread = ft_calloc(sizeof(pthread_t) * tbl->n_philo + 1); // plus 1 for checking deaths
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
	if (create_and_join_threads(tbl, thread, philo, j) == -1)
		return (free(thread), free(j), -1);
	free(j);
	free(thread);
	return (0);
}
