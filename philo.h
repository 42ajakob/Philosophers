/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:46:00 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/12 14:04:16 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_mtx
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	mtx_printf;
	pthread_mutex_t	mtx_last_meal;
	pthread_mutex_t	mtx_n_eaten;
	pthread_mutex_t	mtx_n_eat;
}	t_mtx;

typedef struct s_table
{
	int				n_philo;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	int				n_eat;
	t_mtx			*mtx;
}	t_table;

typedef struct s_philo
{
	int				id;
	long			s_t;
	long			c_t;
	int				n_eaten;
	long			t_die;
	int				t_eat;
	int				t_sleep;
	long			last_meal;
	int				finish;
	t_table			*tbl;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_mtx			*mtx;
}	t_philo;

// check.death.c
void	*check_death(void *arg);

// init_philo_thr.c
t_philo	*init_philo(t_table *tbl);
int		init_thread(t_table *tbl, t_philo *philo);

// init_tbl_mtx.c
t_table	*init_table(int argc, char **argv);

// mutex.c
int		get_printf(t_philo *philo, long time, int id, char *str);
long	get_last_meal(t_philo *philo);
int		get_n_eaten(t_philo *philo);
int		get_n_eat(t_philo *philo);

// philo_utils.c
void	free_all(t_table *tbl, t_philo *philo);
void	*ft_calloc(size_t count);
int		ft_atoi(const char *str);
long	get_time(void);
long	ft_usleep(long mil_sec);

// philo.c
void	*runtime(void *arg);

// validate.c
int		valid_args(int argc, char **argv);

#endif