/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <jtrauque@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:26:06 by julie             #+#    #+#             */
/*   Updated: 2021/07/27 15:26:08 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_on_life(sem_t *semaphore, int *value, int change)
{
	sem_wait(semaphore);
	printf("dead = %d\n", *value);
	if (*value == 1 && change == -1)
	{
		printf("PLOP\n");
		sem_post(semaphore);
		return (FALSE);
	}
	else if (change != -1)
		*value = change;
	sem_post(semaphore);
	return (TRUE);
}

static int	check_allright(t_table *index, t_philo *philo, sem_t *semaphore)
{
	int	i;

	i = 0;
	sem_wait(semaphore);
	while (i < index->nbr_philo
		&& index->each_eat != -1
		&& philo->nbr_meal >= index->each_eat)
	{
		i++;
	}
	if (i == index->nbr_philo)
	{
		print(philo->id, index->philo, END);
		index->allright = 1;
		index->dead = 1;
		return (FALSE);
	}
	sem_post(semaphore);
	return (TRUE);
}

void	*check_death(void *arg)
{
	int		i;
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (1)
	{
		i = -1;
		while (++i < philo->index->nbr_philo && philo->index->dead == 0)
		{
			sem_wait(philo->index->ready);
			if (check_time() - philo->last_meal
				> philo->index->time_die)
			{
				printf("- time = %d\n- last meal = %d\n- time to die = %d\n", check_time(), philo->last_meal, philo->index->time_die);
				printf("->time since the last meal = %d<-\n", check_time() - philo->last_meal); 
				print(philo->id, philo, DEAD);
				philo->index->dead = 1;
				exit(1);
			}
			sem_post(philo->index->ready);
		}
		if (philo->index->dead
			|| check_allright(philo->index, philo, philo->index->ready) == FALSE)
			exit(1);
	}
	return (TRUE);
}
