#include "philo.h"


void	*routine(void *arg)
{
	t_philo	*philo;
	struct	timeval	current_time;

	philo = (t_philo*) arg;
	while (!philo->died)
	{
		pthread_mutex_lock(&philo->forks);
		printf("Philosopher %d has taken a fork\n", philo->id);
		printf("Philosopher %d is eating\n", philo->id);
		philo->nbr_meal++;
		if (philo->index->each_eat[philo->id - 1] == philo->nbr_meal)
			break ;
		gettimeofday(&current_time, NULL);
		philo->last_meal = current_time.tv_sec;
		usleep(philo->index->time_eat);
		printf("Philosopher %d is sleeping\n", philo->id);
		usleep(philo->index->time_sleep);
		printf("Philosopher %d is thinking\n", philo->id);
		pthread_mutex_unlock(&philo->forks);
	}
	pthread_mutex_unlock(&philo->forks);
	return TRUE;
}

int	create_philo(t_table *index)
{
	int	i;
	pthread_t	th[index->nbr_philo];

	i = 0;
	while (i < index->nbr_philo)
	{
		pthread_mutex_init(&index->philo[i].forks, NULL);
		index->philo[i].index = index;
		if (pthread_create(&th[i], NULL, &routine, &index->philo[i]) != 0)
		{
			ft_putstr_fd("Failed to create thread", 2);
			return (FALSE);
		}
		printf("thread %d has started\n", i);
		i++;
	}
	i = 0;
	// check if the philosopher is dead
	while (i < index->nbr_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
			return (FALSE);
		printf("thread %d has finished his execution\n", i);
		pthread_mutex_destroy(&index->philo[i].forks);
		i++;
	}
	return (TRUE);
}
