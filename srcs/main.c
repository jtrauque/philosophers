#include "philo.h"

static int	init_philo(t_table *index)
{
	int	i;

	i = 0;
	index->philo = malloc(sizeof(t_philo) * index->nbr_philo);
	if (index->philo == NULL)
		return FALSE;
	while (i < index->nbr_philo)
	{
		index->philo[i].id = i + 1;
		index->philo[i].nbr_meal = 0;
		index->philo[i].last_meal = 0;
		index->philo[i].died = 0;
		i++;
	}
	return TRUE;
}

static int	init_index(t_table *index, char *argv[], int argc)
{
	int	j;

	j = 1;
	index->nbr_philo = ft_atoi_philo(argv[j++]);
	index->time_die = ft_atoi_philo(argv[j++]);
	index->time_eat = ft_atoi_philo(argv[j++]);
	index->time_sleep = ft_atoi_philo(argv[j++]);
	if (argc == 6)
		index->each_eat = ft_atoi_philo(argv[j++]);
	else 
		index->each_eat = -1;
	if (index->nbr_philo < 0 || index->time_die < 0 || index->time_eat < 0 
			|| index->time_sleep < 0 || (argc == 6 && index->each_eat < 0))
		return (FALSE);
	init_philo(index);
	return (TRUE);
}

int	main(int argc, char *argv[])
{
	t_table index;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("please enter the right amount of arguments", 2);
		return 1;
	}
	if (init_index(&index, argv, argc))
	{
		ft_putstr_fd("an argument is not valid", 2);
		return (FALSE);
	}
	printf("- nbr philo = %d\n- time to die = %d\n- time to eat = %d\n- time to sleep = %d\n", index.nbr_philo, index.time_die, index.time_eat, index.time_sleep);
	for (int i = 0; i < index.nbr_philo; i++)
		printf("philo nbr %d eat %d times\n", i + 1, index.each_eat);
	create_philo(&index);
	free(index.philo);
	return (0);	
}
