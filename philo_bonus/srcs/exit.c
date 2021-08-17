/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <julie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 14:17:49 by julie             #+#    #+#             */
/*   Updated: 2021/08/17 14:17:52 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_exit(t_philo *philo)
{
	sem_close(philo->index->forks);
	sem_close(philo->index->ready);
	sem_close(philo->index->print);
	free(philo->index->philo);
}
