# include "philo.h"

long	get_time(void)
{
	struct timeval	tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); // mil_sec since 1970
}

// int is_philo_dead(t_table *tbl)
// {
// 	if (tbl->t_die) // since eating last time
// 	{
// 		tbl->dead = 1; // has to be protected
// 		return (1);
// 	}
// 	return (0);
// }

long ft_usleep(long mil_sec)
{
	long start;
	long wait;

	start = get_time();
	wait = start + mil_sec;
	while (get_time() < wait) // Include Death Check
		usleep(50);
	return (get_time());
}
