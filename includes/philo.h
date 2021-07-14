#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#define TRUE 0
#define FALSE 1

typedef int	t_bool;

struct	s_table;

typedef struct 	s_philo
{
	int	is_eating;
	int	nbr_meal;
	int last_meal;
	int id;
	int died;
	struct	s_table	*index;
}				t_philo;

typedef struct	s_table
{
	int	nbr_philo;
	int time_eat;
	int time_die;
	int time_sleep;
	int *each_eat;
	int nbr_forks;
	t_philo	*philo;
}				t_table;

/* TOOLS */
int	ft_atoi_philo(char *str);
void	ft_putstr_fd(char *str, int fd);

/* THREAD */
int	create_philo(t_table *index);

#endif 
