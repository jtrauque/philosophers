#include "philo.h"

pthread_mutex_t	mutex;

void	*routine(void *arg)
{
	t_table	*philo;

	philo = (t_table*) arg;
	while (!index->died)
	{
		pthread_mutex_lock(&index->nbr_forks);
		printf("Philosopher %d has forks !\n", index);
		printf("Philosopher %d is eating !\n", index);
		pthread_mutex_unlock(&mutex);
	}
	return TRUE;
}

int	create_philo(t_table *index)
{
	int	i;
	pthread_t	th[index->nbr_philo];

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	while (i < index->nbr_philo)
	{
		if (pthread_create(&th[i], NULL, &routine, &index->philo[i]) != 0)
		{
			ft_putstr_fd("Failed to create thread", 2);
			return (FALSE);
		}
		printf("thread %d has started\n", i);
		i++;
	}
	i = 0;
	while (i < index->nbr_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
			return (FALSE);
		printf("thread %d has finished his execution\n", i);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	return (TRUE);
}
