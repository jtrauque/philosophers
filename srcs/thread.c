/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <jtrauque@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:26:25 by julie             #+#    #+#             */
/*   Updated: 2021/07/27 15:26:30 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	release_fork(t_philo *philo)
{
	check_mutex(&philo->left_fork->fork, &philo->left_fork->take, 0);
	check_mutex(&philo->right_fork.fork, &philo->right_fork.take, 0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (check_mutex(philo->ready, &philo->index->dead, -1) == TRUE)
	{
		if (premission_to_left(philo->left_fork) == 0
			|| premission_to_right(philo->left_fork, &philo->right_fork) == 0)
			continue ;
		print(philo->id, philo, EAT);
		check_mutex(philo->ready, &philo->nbr_meal, philo->nbr_meal + 1);
		check_mutex(philo->ready, &philo->last_meal, check_time());
		if (check_mutex(philo->ready, &philo->index->dead, -1))
			return (0);
		usleep(philo->index->time_eat * 1000);
		if (check_mutex(philo->ready, &philo->index->dead, -1))
			return (0);
		print(philo->id, philo, SLEEP);
		release_fork(philo);
		usleep(philo->index->time_sleep * 1000);
		if (check_mutex(philo->ready, &philo->index->dead, -1))
			return (0);
		print(philo->id, philo, THINK);
		usleep(100);
	}
	return (TRUE);
}

void	init_thread(t_table *index, pthread_mutex_t *print,
		pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	pthread_mutex_init(print, NULL);
	pthread_mutex_init(meal, NULL);
	while (i < index->nbr_philo)
	{
		pthread_mutex_init(&index->philo[i].right_fork.fork, NULL);
		index->philo[i].print = print;
		index->philo[i].ready = meal;
		index->philo[i].index = index;
		if (i == index->nbr_philo - 1)
			index->philo[i].left_fork = &index->philo[0].right_fork;
		else
			index->philo[i].left_fork = &index->philo[i + 1].right_fork;
		index->philo[i].right_fork.take = 0;
		i++;
	}
}

int	end_of_simulation(t_table *index, pthread_t *th, pthread_mutex_t *print,
		pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	while (i < index->nbr_philo)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			ft_putstr_fd("Failed to join your thread", 2);
			return (FALSE);
		}
		i++;
	}
	i = 0;
	while (i < index->nbr_philo)
	{
		pthread_mutex_destroy(&index->philo[i].right_fork.fork);
		i++;
	}
	pthread_mutex_destroy(print);
	pthread_mutex_destroy(meal);
	return (TRUE);
}

int	create_philo(t_table *index)
{
	int				i;
	pthread_t		*th;
	pthread_mutex_t	print_action;
	pthread_mutex_t	meal;

	i = 0;
	th = malloc_thread(index->nbr_philo);
	init_thread(index, &print_action, &meal);
	check_time();
	while (i < index->nbr_philo)
	{
		if (pthread_create(&th[i], NULL, &routine, &index->philo[i]) != 0)
		{
			ft_putstr_fd("Failed to create thread", 2);
			free(th);
			return (FALSE);
		}
		i++;
	}
	check_death(index, &meal);
	end_of_simulation(index, th, &print_action, &meal);
	free(th);
	return (TRUE);
}
