/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:12:15 by vpoelman          #+#    #+#             */
/*   Updated: 2025/09/11 15:12:15 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	validate_args(int argc, char **argv, int *args)
{
	int	i;
	int	j;
	int	val;

	if (argc < 5 || argc > 6)
		return (0);
	i = 1;
	while (i < argc)
	{
		if (!argv[i][0])
			return (0);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		val = ft_atoi(argv[i]);
		args[i - 1] = val;
		if (val <= 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	int	args[5];

	if (!validate_args(argc, argv, args))
		return (1);
	data->num_philos = args[0];
	data->time_to_die = args[1];
	data->time_to_eat = args[2];
	data->time_to_sleep = args[3];
	data->num_meals = -1;
	if (argc == 6)
		data->num_meals = args[4];
	data->is_dead = 0;
	data->start_time = get_time();
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);
	data->forks = NULL;
	return (0);
}

static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			data->num_philos = i;
			cleanup_forks(data);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (init_forks(data))
		return (1);
	if (pthread_mutex_init(&data->print_mutex, NULL))
	{
		cleanup_forks(data);
		return (1);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL))
	{
		pthread_mutex_destroy(&data->print_mutex);
		cleanup_forks(data);
		return (1);
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].finished = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		i++;
	}
	return (0);
}
