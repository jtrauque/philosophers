#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#define TRUE 0
#define FALSE 1


typedef	enum e_arg
{
	FORK = 2,
	EAT,
	SLEEP,
	THINK,
	DEAD,
	END
}			t_arg;

typedef int	t_bool;

struct	s_table;

typedef	struct	s_protect
{
	pthread_mutex_t	fork;
	int take;
}				t_protect;

typedef struct 	s_philo
{
	int	is_eating;
	int	nbr_meal;
	int last_meal;
	int id;
	int died;
	t_protect	*left_fork;
	t_protect	right_fork; //fourchette du voisin
	pthread_mutex_t	*print;
	pthread_mutex_t	*ready;
	struct	s_table	*index;
}				t_philo;

typedef struct	s_table
{
	int	nbr_philo;
	int time_eat;
	int time_die;
	int time_sleep;
	int each_eat;
	int nbr_forks;
	int dead;
	int allright;
	t_philo	*philo;
}				t_table;

/* TOOLS */
int	ft_atoi_philo(char *str);
void	ft_putstr_fd(char *str, int fd);
int	check_time(void);
void	print(int id, t_philo *philo, int action);

/* THREAD */
int	create_philo(t_table *index);
void	release_fork(t_philo *philo);

#endif 
