/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julie <jtrauque@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:26:56 by julie             #+#    #+#             */
/*   Updated: 2021/07/27 15:27:04 by julie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

# define TRUE 0
# define FALSE 1

typedef enum e_arg
{
	FORK = 2,
	EAT,
	SLEEP,
	THINK,
	DEAD,
	END
}			t_arg;

struct	s_table;

typedef struct s_philo
{
	int				is_eating;
	int				nbr_meal;
	int				last_meal;
	int				id;
	int				died;
	pid_t			pid_id;
	sem_t			*ready;
	pthread_t		th;
	struct s_table	*index;
}				t_philo;

typedef struct s_table
{
	int		nbr_philo;
	int		time_eat;
	int		time_die;
	int		time_sleep;
	int		each_eat;
	int		nbr_forks;
	int		dead;
	int		allright;
	sem_t	*forks;
	sem_t	*ready;
	sem_t	*print;
	t_philo	*philo;
}				t_table;

/* TOOLS */
int			ft_atoi_philo(char *str);
void		ft_putstr_fd(char *str, int fd);
int			check_time(void);
void		print(int id, t_philo *philo, int action);
pthread_t	*malloc_thread(int nbr);

/* THREAD */
int			create_philo(t_table *index);
void		release_fork(t_philo *philo);

/* CHECK */
int	check_on_life(sem_t *semaphore, int *value, int change);
void	*check_death(void *arg);
int	check_allright(t_table *index, sem_t *semaphore);
/* int	premission_to_left(t_protect *fork); */
/* int	premission_to_right(t_protect *left_fork, t_protect *right_fork); */
#endif 
