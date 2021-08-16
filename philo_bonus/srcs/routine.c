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

static void	release(t_philo *philo)
{
	sem_post(philo->index->forks);
	sem_post(philo->index->forks);
}

static int	eat(t_philo *philo)
{
	sem_wait(philo->index->forks);
	print(philo->id, philo, FORK);
	sem_wait(philo->index->forks);
	print(philo->id, philo, FORK);
	print(philo->id, philo, EAT);
	check_on_life(philo->index->ready, &philo->nbr_meal,
		philo->nbr_meal + 1);
	check_on_life(philo->index->ready, &philo->last_meal, check_time());
	if (check_on_life(philo->index->ready, &philo->index->dead, -1))
	{
		release(philo);
		return (1);
	}
	return (0);
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
		pthread_detach(philo->th);
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

void	think_opti(t_philo *philo)
{
	int time;
	int thinking_time;

	time = check_time();
	thinking_time = philo->index->time_die - (time - philo->last_meal) - 10 > 0;
	print(philo->id, philo, THINK);
	if (thinking_time > 0)
		usleep(thinking_time * 1000);
}

void	*routine(t_philo *philo)
{
	thread_manager(philo, 0);
	sem_wait(philo->index->ready);
	sem_post(philo->index->ready);
	if (philo->id % 2 != 0)
		usleep(1000);
	if (philo->id == philo->index->nbr_philo && philo->id % 2 != 0)
		usleep(1000);
	while (check_on_life(philo->index->ready, &philo->index->dead, -1) == TRUE)
	{
		if(eat(philo))
			break ;
		usleep(philo->index->time_eat * 1000);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
		{
			release(philo);
			break ;
		}
		print(philo->id, philo, SLEEP);
		release(philo);
		usleep(philo->index->time_sleep * 1000);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
			break ;
		think_opti(philo);
	}
	/* thread_manager(philo, 1); */
	return (TRUE);
}
