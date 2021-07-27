/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <jtrauque@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:26:34 by julie             #+#    #+#             */
/*   Updated: 2021/07/27 15:26:37 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(fd, str, i);
	write(fd, "\n", 1);
}

int	ft_atoi_philo(char *str)
{
	int		i;
	long	n;

	i = 0;
	n = 0;
	if (str[i] == '-')
		return (-1);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		n = n * 10 + (str[i++] - '0');
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (-2);
	return (n);
}

int	check_time(void)
{
	struct timeval	current_time;
	static int		start_time = 0;

	if (gettimeofday(&current_time, NULL) == -1)
		return (-1);
	if (!start_time)
		start_time = (current_time.tv_sec * 1000)
			+ (current_time.tv_usec / 1000);
	return ((current_time.tv_sec * 1000)
		+ (current_time.tv_usec / 1000) - start_time);
}

pthread_t	*malloc_thread(int nbr)
{
	pthread_t	*th;
	int			i;

	i = 0;
	th = malloc(sizeof(pthread_t) * nbr);
	if (th == NULL)
		return (NULL);
	return (th);
}

void	print(int id, t_philo *philo, int action)
{
	pthread_mutex_lock(philo->print);
	if (action == FORK)
		printf("%d : Philosopher %d has taken a fork\n", check_time(), id);
	else if (action == EAT)
	{
		printf("%d : Philosopher %d has taken a fork\n", check_time(), id);
		printf("%d : Philosopher %d has taken a fork\n", check_time(), id);
		printf("%d : Philosopher %d is eating\n", check_time(), id);
	}
	else if (action == SLEEP)
	{
		printf("%d : Philosopher %d is sleeping\n", check_time(), id);
	}
	else if (action == THINK)
		printf("%d : Philosopher %d is thinking\n", check_time(), id);
	else if (action == DEAD)
		printf("%d : Philosopher %d is dead\n", check_time(), id);
	else
		printf("%d : Everyone has eaten : End of simulation\n", check_time());
	pthread_mutex_unlock(philo->print);
}
