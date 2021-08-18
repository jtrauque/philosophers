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

static int	sleep_and_think(t_philo *philo)
{
	int	time;
	int	thinking_time;

	time = check_time();
	count_down(philo->index->time_eat);
	if (check_mutex(philo->ready, &philo->index->dead, -1) == FALSE)
		return (1);
	print(philo->id, philo, SLEEP, 0);
	release_fork(philo);
	count_down(philo->index->time_sleep);
	if (check_mutex(philo->ready, &philo->index->dead, -1) == FALSE)
		return (1);
	/* thinking_time = philo->index->time_die - (time - philo->last_meal) - 30; */
	thinking_time = (philo->last_meal + philo->index->time_eat * (1 + (philo->id % 2)) + philo->index->time_sleep) - check_time() - 10 * 1000;
	print(philo->id, philo, THINK, time);
	if (thinking_time > 0)
		count_down(thinking_time);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (check_mutex(philo->ready, &philo->index->dead, -1) == TRUE)
	{
		if (premission_to_left(philo->left_fork) == 0
			|| premission_to_right(philo->left_fork, &philo->right_fork) == 0)
		{
			usleep(50);
			continue ;
		}
		pthread_mutex_lock(philo->ready);
		if (philo->index->dead == 1)
		{
			pthread_mutex_unlock(philo->ready);
			break ;
		}
		philo->last_meal = check_time();
		print(philo->id, philo, EAT, philo->last_meal);
		philo->nbr_meal += 1;
		pthread_mutex_unlock(philo->ready);
		if (sleep_and_think(philo) == 1)
			break ;
	}
	return (0);
}

int	end_of_simulation(t_table *index, pthread_mutex_t *print,
		pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	while (i < index->fail)
	{
		if (pthread_join(index->th[i], NULL) != 0)
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
	pthread_mutex_t	print_action;
	pthread_mutex_t	meal;

	i = 0;
	index->th = malloc_thread(index->nbr_philo);
	init_thread(index, &print_action, &meal);
	check_time();
	while (i < index->nbr_philo)
	{
		if (pthread_create(&index->th[i], NULL, &routine,
				&index->philo[i]) != 0)
		{
			ft_putstr_fd("Failed to create thread", 2);
			index->fail = i - 1;
			end_of_simulation(index, &print_action, &meal);
			free(index->th);
			return (FALSE);
		}
		i++;
	}
	check_death(index, &meal);
	end_of_simulation(index, &print_action, &meal);
	free(index->th);
	return (TRUE);
}
