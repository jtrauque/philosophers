#include "philo.h"

int	premission_to_left(t_protect *fork)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&fork->fork);
	if (fork->take == 0)
	{
		fork->take = 1;
		ret = fork->take;
	}
	pthread_mutex_unlock(&fork->fork);
	return (ret);
}

int	premission_to_right(t_protect *left_fork, t_protect *right_fork)
{
	int	ret;
	int	plop;

	ret = 0;
	pthread_mutex_lock(&right_fork->fork);
	plop = right_fork->take;
	if (right_fork->take == 0)
	{
		right_fork->take = 1;
		ret = right_fork->take;
	}
	pthread_mutex_unlock(&right_fork->fork);
	pthread_mutex_lock(&left_fork->fork);
	if (plop == 1)
	{
		left_fork->take = 0;
	}
	pthread_mutex_unlock(&left_fork->fork);
	return (ret);
}

void	release_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->fork);
	philo->left_fork->take = 0;
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_lock(&philo->right_fork.fork);
	philo->right_fork.take = 0;
	pthread_mutex_unlock(&philo->right_fork.fork);
}

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(philo->ready);
	while (!philo->index->dead)
	{
		pthread_mutex_unlock(philo->ready);
		if (premission_to_left(philo->left_fork) == 0)
		{
			pthread_mutex_lock(philo->ready);
			continue ;
		}
		if (premission_to_right(philo->left_fork, &philo->right_fork) == 0)
		{
			pthread_mutex_lock(philo->ready);
			continue ;
		}
		print(philo->id, philo, FORK);
		print(philo->id, philo, FORK);
		print(philo->id, philo, EAT);
		pthread_mutex_lock(philo->ready);
		philo->nbr_meal++;
		philo->last_meal = check_time();
		if (philo->index->dead)
			return (0);
		pthread_mutex_unlock(philo->ready);
		usleep(philo->index->time_eat * 1000);
		print(philo->id, philo, SLEEP);
		release_fork(philo);
		usleep(philo->index->time_sleep * 1000);
		print(philo->id, philo, THINK);
		pthread_mutex_lock(philo->ready);
	}
	pthread_mutex_unlock(philo->ready);
	return (TRUE);
}

void	check_death(t_table *index, pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	while (!index->allright)
	{
		i = 0;
		while (i < index->nbr_philo && index->dead == 0)
		{
			pthread_mutex_lock(meal);
			if (check_time() - index->philo[i].last_meal > index->time_die)
			{
				print(index->philo[i].id, index->philo, DEAD);
				index->dead = 1;
			}
			pthread_mutex_unlock(meal);
			i++;
		}
		if (index->dead)
			break ;
		i = 0;
		pthread_mutex_lock(meal);
		while (i < index->nbr_philo 
				&& index->philo[i].nbr_meal == index->each_eat)	
			i++;
		pthread_mutex_unlock(meal);
		if (i == index->nbr_philo)
		{
			print(index->philo[i].id, index->philo, END);
			index->allright = 1;
			break ;
		}
	}
}

pthread_t	*init_thread(int nbr)
{
	pthread_t	*th;
	int	i;

	i = 0;
	th = malloc(sizeof(pthread_t) * nbr);
	if (th == NULL)
		return (NULL);
	return (th);
}
int	create_philo(t_table *index)
{
	int				i;
	pthread_t		*th;
	pthread_mutex_t	print_action;
	pthread_mutex_t	meal;

	i = 0;
	th = init_thread(index->nbr_philo);
	pthread_mutex_init(&print_action, NULL);
	pthread_mutex_init(&meal, NULL);
	while (i < index->nbr_philo)
	{
		pthread_mutex_init(&index->philo[i].right_fork.fork, NULL);
		index->philo[i].print = &print_action;
		index->philo[i].ready = &meal;
		index->philo[i].index = index;
		if (i == index->nbr_philo - 1)
			index->philo[i].left_fork = &index->philo[0].right_fork;
		else
			index->philo[i].left_fork = &index->philo[i + 1].right_fork;
		// pointeur sur la fourchette du voisin
		index->philo[i].right_fork.take = 0;
		i++;
	}
	i = 0;
	check_time();
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
	check_death(index, &meal);
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
		pthread_mutex_destroy(&index->philo[i].right_fork.fork);
		i++;
	}
	pthread_mutex_destroy(&print_action);
	pthread_mutex_destroy(&meal);
	free(th);
	return (TRUE);
}
