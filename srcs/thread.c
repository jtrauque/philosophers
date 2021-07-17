#include "philo.h"

void	print(int id, t_philo *philo, int action)
{
	pthread_mutex_lock(philo->print);
	if (action == FORK)
		printf("Philosopher %d has taken a fork\n", id);
	else if (action == EAT)
		printf("Philosopher %d is eating\n", id);
	else if (action == SLEEP)
		printf("Philosopher %d is sleeping\n", id);
	else
		printf("Philosopher %d is thinking\n", id);
	pthread_mutex_unlock(philo->print);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	struct timeval	current_time;

	philo = (t_philo *) arg;
	while (!philo->died)
	{
		pthread_mutex_lock(philo->right_fork);
		print(philo->id, philo, FORK);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		print(philo->id, philo, FORK);
		print(philo->id, philo, EAT);
		philo->nbr_meal++;
		gettimeofday(&current_time, NULL);
		philo->last_meal = current_time.tv_sec;
		usleep(philo->index->time_eat * 1000);
		if (philo->index->each_eat == philo->nbr_meal)
			break ;
		print(philo->id, philo, SLEEP);
		pthread_mutex_unlock(philo->left_fork);
		usleep(philo->index->time_sleep * 1000);
		print(philo->id, philo, THINK);
	}
	pthread_mutex_unlock(philo->left_fork);
	return (TRUE);
}

int	create_philo(t_table *index)
{
	int				i;
	pthread_t		th[index->nbr_philo];
	pthread_mutex_t	forks[index->nbr_philo];
	pthread_mutex_t	print_action;

	i = 0;
	/* memset(&th, 0, sizeof(int) * index->nbr_philo); */
	pthread_mutex_init(&print_action, NULL);
	while (i < index->nbr_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		index->philo[i].print = &print_action;
		i++;
	}
	i = 0;
	while (i < index->nbr_philo)
	{
		index->philo[i].index = index;
		if (i == index->nbr_philo - 1)
			index->philo[i].left_fork = &forks[0];
		else
			index->philo[i].left_fork = &forks[i + 1];
		index->philo[i].right_fork = &forks[i];
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
		i++;
	}
	i = 0;
	while (i < index->nbr_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&print_action);
	return (TRUE);
}
