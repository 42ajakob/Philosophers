/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:46:00 by ajakob            #+#    #+#             */
/*   Updated: 2024/03/10 18:43:54 by ajakob           ###   ########.fr       */
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

typedef struct s_table
{
	int				n_philo;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	int				n_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t mutex_printf;
	pthread_mutex_t mutex_eaten;
	pthread_mutex_t mutex_last_meal;
}	t_table;

typedef struct s_philo
{
	int				id;
	long			sta_time;
	long			cur_time;
	int				n_eaten;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	long			last_meal;
	int				dead;
	t_table			*tbl;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*mutex_printf;
	pthread_mutex_t	*mutex_eaten;
	pthread_mutex_t	*mutex_last_meal;
}	t_philo;

// time.c
long	get_time(void);
long	ft_usleep(long mil_sec);

// philo_utils.c
void	free_all(t_table *tbl, t_philo *philo);
void	*ft_calloc(size_t count);
int		ft_atoi(const char *str);

// init.c
t_table	*init_table(int argc, char **argv);
t_philo	*init_philo(t_table *tbl);
void	sort_mutex_pointers(t_table *tbl, t_philo *philo, int i);
int		init_thread(t_table *tbl, t_philo *philo);


// validate.c
int		valid_args(int argc, char **argv);

// philo.c
void	*runtime(void *arg);
void	*check_death(void *arg);

#endif