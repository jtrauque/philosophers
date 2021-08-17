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
	int	old_stat;

	ret = 0;
	pthread_mutex_lock(&right_fork->fork);
	old_stat = right_fork->take;
	if (right_fork->take == 0)
	{
		right_fork->take = 1;
		ret = right_fork->take;
	}
	pthread_mutex_unlock(&right_fork->fork);
	if (old_stat == 1)
		check_mutex(&left_fork->fork, &left_fork->take, 0);
	return (ret);
}

int	check_mutex(pthread_mutex_t *mutex, int *value, int change)
{
	pthread_mutex_lock(mutex);
	if (*value == 1 && change == -1)
	{
		pthread_mutex_unlock(mutex);
		return (FALSE);
	}
	else if (change != -1)
		*value = change;
	pthread_mutex_unlock(mutex);
	return (TRUE);
}

void	check_allright(t_table *index, pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	pthread_mutex_lock(meal);
	while (i < index->nbr_philo
		&& index->each_eat != -1
		&& index->philo[i].nbr_meal >= index->each_eat)
	{
		i++;
	}
	if (i == index->nbr_philo)
	{
		print(index->philo[i - 1].id, index->philo, END);
		index->allright = 1;
		index->dead = 1;
	}
	pthread_mutex_unlock(meal);
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
		check_allright(index, meal);
		if (index->dead)
			break ;
		usleep(1000);
	}
}
