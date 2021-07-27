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

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (check_on_life(&philo->index->semaphore, &philo->index->dead, -1) == TRUE)
	{
		/* if (premission_to_left(philo->left_fork) == 0 */
		/* 	|| premission_to_right(philo->left_fork, &philo->right_fork) == 0) */
		/* 	continue ; */
		print(philo->id, philo, EAT);
		check_on_life(&philo->index->semaphore, &philo->nbr_meal, philo->nbr_meal + 1);
		check_on_life(&philo->index->semaphore, &philo->last_meal, check_time());
		if (check_on_life(&philo->index->semaphore, &philo->index->dead, -1))
			return (0);
		usleep(philo->index->time_eat * 1000);
		if (check_on_life(&philo->index->semaphore, &philo->index->dead, -1))
			return (0);
		print(philo->id, philo, SLEEP);
		/* release_fork(philo); */
		usleep(philo->index->time_sleep * 1000);
		if (check_on_life(&philo->index->semaphore, &philo->index->dead, -1))
			return (0);
		print(philo->id, philo, THINK);
		usleep(100);
	}
	return (TRUE);
}

int	end_of_simulation(t_table *index, pthread_t *th)
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
	sem_destroy(&index->semaphore);
	return (TRUE);
}

int	create_philo(t_table *index)
{
	int				i;
	pthread_t		*th;

	i = -1;
	th = malloc_thread(index->nbr_philo);
	sem_init(&index->semaphore, 0, index->nbr_philo);
	while (++i < index->nbr_philo)
		index->philo[i].index = index;
	i = 0;
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
	check_death(index, &index->semaphore);
	end_of_simulation(index, th);
	free(th);
	return (TRUE);
}
