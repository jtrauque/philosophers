/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <julie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 12:24:58 by julie             #+#    #+#             */
/*   Updated: 2021/08/17 12:25:01 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_thread(t_table *index, pthread_mutex_t *print,
		pthread_mutex_t *meal)
{
	int	i;

	i = 0;
	index->fail = index->nbr_philo;
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

void	count_down(int ring)
{
	int	time;
	int	obj;

	time = check_time();
	obj = time + ring;
	if (ring > 10)
		usleep((ring - 10) * 1000);
	printf("%d - spin lock\n", check_time());
	while (check_time() < time + ring)
		;
}
