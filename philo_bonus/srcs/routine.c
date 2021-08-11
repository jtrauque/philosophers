/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <julie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 17:07:01 by julie             #+#    #+#             */
/*   Updated: 2021/08/11 17:07:04 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	sem_wait(philo->index->forks);
	print(philo->id, philo, FORK);
	sem_wait(philo->index->forks);
	print(philo->id, philo, FORK);
	print(philo->id, philo, EAT);
	check_on_life(philo->index->ready, &philo->nbr_meal,
		philo->nbr_meal + 1);
	check_on_life(philo->index->ready, &philo->last_meal, check_time());
}

static void	release(t_philo *philo, int dead)
{
	if (dead == 0)
	{
		print(philo->id, philo, SLEEP);
		sem_post(philo->index->forks);
		sem_post(philo->index->forks);
		usleep(philo->index->time_sleep * 1000);
	}
	else
	{
		sem_post(philo->index->forks);
		sem_post(philo->index->forks);
	}
}

static void	thread_manager(t_philo *philo, int action)
{
	if (action == 0)
	{
		if (pthread_create(&philo->th, NULL, &check_death, philo) != 0)
		{
			ft_putstr_fd("Failed to create thread", 2);
			exit(1);
		}
	}
	else
	{
		if (pthread_join(philo->th, NULL) != 0)
		{
			ft_putstr_fd("Failed to join your thread", 2);
			exit(1);
		}
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	thread_manager(philo, 0);
	while (check_on_life(philo->index->ready, &philo->index->dead, -1) == TRUE)
	{
		eat(philo);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
		{
			release(philo, 1);
			break ;
		}
		usleep(philo->index->time_eat * 1000);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
		{
			release(philo, 1);
			break ;
		}
		release(philo, 0);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
			break ;
		print(philo->id, philo, THINK);
	}
	thread_manager(philo, 1);
	return (TRUE);
}
