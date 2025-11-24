/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 23:41:25 by vpoelman          #+#    #+#             */
/*   Updated: 2025/11/24 23:41:30 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	check_all_philos_finished(t_data *data)
{
	int	i;
	int	all_eaten;

	if (data->num_meals == -1)
		return (0);
	pthread_mutex_lock(&data->death_mutex);
	i = 0;
	all_eaten = 1;
	while (i < data->num_philos)
	{
		if (!data->philos[i].finished)
		{
			all_eaten = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (all_eaten);
}

static int	check_one_philo(t_data *data, int i, long long time)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->is_dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (0);
	}
	if (!data->philos[i].finished && time
		- data->philos[i].last_meal_time > data->time_to_die)
	{
		data->is_dead = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", time - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (-1);
}

static int	check_philo_death(t_data *data)
{
	int			i;
	int			result;
	long long	time;

	time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		result = check_one_philo(data, i, time);
		if (result != -1)
			return (result);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_all_philos_finished(data))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->is_dead = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		if (check_philo_death(data))
			return (NULL);
		usleep(1000);
	}
}
