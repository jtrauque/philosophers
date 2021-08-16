/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <jtrauque@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:26:15 by julie             #+#    #+#             */
/*   Updated: 2021/07/27 15:26:21 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo(t_table *index)
{
	int	i;

	i = 0;
	index->philo = malloc(sizeof(t_philo) * index->nbr_philo);
	if (index->philo == NULL)
		return (FALSE);
	while (i < index->nbr_philo)
	{
		index->philo[i].id = i + 1;
		index->philo[i].nbr_meal = 0;
		index->philo[i].last_meal = 0;
		index->philo[i].died = 0;
		i++;
	}
	return (TRUE);
}

static int	init_index(t_table *index, char *argv[], int argc)
{
	int	j;

	j = 1;
	index->nbr_philo = ft_atoi_philo(argv[j++]);
	index->time_die = ft_atoi_philo(argv[j++]);
	index->time_eat = ft_atoi_philo(argv[j++]);
	index->time_sleep = ft_atoi_philo(argv[j++]);
	index->allright = 0;
	index->dead = 0;
	if (argc == 6)
		index->each_eat = ft_atoi_philo(argv[j++]);
	else
		index->each_eat = -1;
	if (index->nbr_philo <= 0 || index->nbr_philo > 200 || index->time_die < 0
		|| index->time_eat < 0 || index->time_sleep < 0
		|| (argc == 6 && index->each_eat < 0))
		return (FALSE);
	init_philo(index);
	return (TRUE);
}

int	main(int argc, char *argv[])
{
	t_table	index;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("please enter the right amount of arguments", 2);
		return (FALSE);
	}
	if (init_index(&index, argv, argc))
	{
		ft_putstr_fd("an argument is not valid", 2);
		return (FALSE);
	}
	create_philo(&index);
	free(index.philo);
	return (TRUE);
}
