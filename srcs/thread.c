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
	else if (action == THINK)
		printf("Philosopher %d is thinking\n", id);
	else 
		printf("Philosopher %d is dead\n", id);
	pthread_mutex_unlock(philo->print);
}

int	check_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL) == -1)
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *) arg;
	while (!philo->died)
	{
		print(philo->id, philo, THINK);
		if (philo->id % 2)
		{
			pthread_mutex_lock(philo->right_fork);
			print(philo->id, philo, FORK);
			pthread_mutex_lock(philo->left_fork);
		}
		else 
		{
			pthread_mutex_lock(philo->left_fork);
			print(philo->id, philo, FORK);
			pthread_mutex_lock(philo->right_fork);
		}
		print(philo->id, philo, FORK);
		print(philo->id, philo, EAT);
		philo->nbr_meal++;
		philo->last_meal = check_time();
		printf("time = %d\n", philo->last_meal);
		usleep(philo->index->time_eat * 1000);
		print(philo->id, philo, SLEEP);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		usleep(philo->index->time_sleep * 1000);
		/* print(philo->id, philo, THINK); */
		if (philo->index->each_eat == philo->nbr_meal)
			break ;
	}
	while (check_time() - philo->last_meal < philo->index->time_die)
		;
	print(philo->id, philo, DEAD);
	/* philo->index->dead = 1; */
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
