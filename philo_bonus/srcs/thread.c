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

static int	end_of_simulation(t_table *index)
{
	int	i;

	i = 0;
	waitpid(-1, NULL, 0);
	while (i < index->nbr_philo)
	{
		kill(index->philo[i].pid_id, SIGKILL);
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

static int	init_sem(t_table *index)
{
	sem_unlink("/forks");
	sem_unlink("/ready");
	sem_unlink("/print");
	index->forks = sem_open("/forks", O_CREAT, S_IRWXU, index->nbr_philo);
	index->ready = sem_open("/ready", O_CREAT, S_IRWXU, 1);
	index->print = sem_open("/print", O_CREAT, S_IRWXU, 1);
	if (index->forks <= 0 || index->ready <= 0 || index->print <= 0)
		return (FALSE);
	return (TRUE);
}

int	create_philo(t_table *index)
{
	int	i;

	i = -1;
	if (init_sem(index))
		return (FALSE);
	while (++i < index->nbr_philo)
		index->philo[i].index = index;
	i = -1;
	check_time();
	sem_wait(index->ready);
	while (++i < index->nbr_philo)
	{
		index->philo[i].pid_id = fork();
		if (index->philo[i].pid_id < 0)
		{
			ft_putstr_fd("Failed to create a fork", 2);
			sem_post(index->ready);
			return (FALSE);
		}
		else if (index->philo[i].pid_id == 0)
			routine(&index->philo[i]);
	}
	sem_post(index->ready);
	end_of_simulation(index);
	return (TRUE);
}
