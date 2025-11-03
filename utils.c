/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+        */
/*   Created: 2025/09/11 15:12:42 by vpoelman          #+#    #+#             */
/*   Updated: 2025/09/11 15:12:42 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long long	get_time(void)
{
	struct timeval	v;

	gettimeofday(&v, NULL);
	return (v.tv_sec * 1000 + v.tv_usec / 1000);
}

void	print_status(t_philo *philo, char *status)
{
	long long	time;
	int			is_dead;

	pthread_mutex_lock(&philo->data->death_mutex);
	is_dead = philo->data->is_dead;
	if (!is_dead)
		time = get_time() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (is_dead)
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%lld %d %s\n", time, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	ft_usleep(long long time)
{
	long long	target;

	target = get_time() + time;
	while (get_time() < target)
		usleep(50);
}

void	cleanup_mutexes(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
