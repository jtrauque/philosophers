#include "philo.h"

static int	init_philo(t_table *index)
{
	int	i;

	i = 0;
	while (i < index->nbr_philo)
	{
		index.philo[i]->id
	}
}
static int	init_index(t_table *index, char *argv[])
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	index->nbr_philo = ft_atoi_philo(argv[j++]);
	index->time_die = ft_atoi_philo(argv[j++]);
	index->time_eat = ft_atoi_philo(argv[j++]);
	index->time_sleep = ft_atoi_philo(argv[j++]);
	index->nbr_forks = index->nbr_philo / 2;
	index->each_eat = malloc(sizeof(int) * index->nbr_philo);
	if (index->each_eat == NULL)
		return (FALSE);
	while (i < index->nbr_philo)
	{
		index->each_eat[i] = ft_atoi_philo(argv[j]);
		if (index->each_eat[i] < 0)
			return (FALSE);
		i++;
		j++;
	}
	if (index->nbr_philo < 0 || index->time_die < 0 || index->time_eat < 0 
			|| index->time_sleep < 0 || i != index->nbr_philo 
			|| argv[j])
		return (FALSE);
	init_philo(index);
	return (TRUE);
}

int	main(int argc, char *argv[])
{
	t_table index;

	if (argc < 5)
	{
		ft_putstr_fd("please enter the right amount of arguments", 2);
		return 1;
	}
	if (init_index(&index, argv))
	{
		ft_putstr_fd("an argument is not valid", 2);
		free(index.each_eat);
		return (FALSE);
	}
	printf("- nbr philo = %d\n- time to die = %d\n- time to eat = %d\n- time to sleep = %d\n", index.nbr_philo, index.time_die, index.time_eat, index.time_sleep);
	for (int i = 0; i < index.nbr_philo; i++)
		printf("philo nbr %d eat %d times\n", i + 1, index.each_eat[i]);
	create_philo(&index);
	free(index.each_eat);
	return (0);	
}
