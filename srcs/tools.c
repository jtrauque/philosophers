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
