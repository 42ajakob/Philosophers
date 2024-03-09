# include "philo.h"

int	valid_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < 48 || argv[i][j] > 57)
				return (EXIT_FAILURE);
			j++;
		}
		i++;
	}
	if (ft_atoi(argv[1]) > 200)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
