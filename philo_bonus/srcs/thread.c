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
	while (check_on_life(philo->index->ready, &philo->index->dead, -1) == TRUE)
	{
		sem_wait(philo->index->forks);
		print(philo->id, philo, FORK);
		print(philo->id, philo, FORK);
		print(philo->id, philo, EAT);
		check_on_life(philo->index->ready, &philo->nbr_meal, philo->nbr_meal + 1);
		check_on_life(philo->index->ready, &philo->last_meal, check_time());
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
		{
			sem_post(philo->index->forks);
			return (0);
		}
		usleep(philo->index->time_eat * 1000);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
		{
			sem_post(philo->index->forks);
			return (0);
		}
		print(philo->id, philo, SLEEP);
		sem_post(philo->index->forks);
		usleep(philo->index->time_sleep * 1000);
		if (check_on_life(philo->index->ready, &philo->index->dead, -1))
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
	sem_close(index->forks);
	sem_close(index->ready);
	sem_close(index->print);
	sem_unlink("/forks");
	sem_unlink("/ready");
	sem_unlink("/print");
	return (TRUE);
}

int	create_philo(t_table *index)
{
	int				i;
	pthread_t		*th;

	i = -1;
	sem_unlink("/forks");
	sem_unlink("/ready");
	sem_unlink("/print");
	index->forks = sem_open("/forks", O_CREAT, S_IRWXU, index->nbr_philo / 2);
	index->ready = sem_open("/ready", O_CREAT, S_IRWXU, 1);
	index->print = sem_open("/print", O_CREAT, S_IRWXU, 1);
	if (index->forks <= 0 || index->ready <= 0 || index->print <= 0)
		return (FALSE);
	th = malloc_thread(index->nbr_philo);
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
	check_death(index, index->ready);
	end_of_simulation(index, th);
	free(th);
	return (TRUE);
}
