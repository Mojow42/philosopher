/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:12:19 by vpoelman          #+#    #+#             */
/*   Updated: 2025/09/11 15:12:19 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	cleanup(t_data *data)
{
	cleanup_mutexes(data);
	if (data->philos)
		free(data->philos);
}

static int	init_simulation_data(t_data *data, int argc, char **argv)
{
	if (init_data(data, argc, argv))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (init_mutexes(data))
	{
		printf("Error: mutex initialization failed\n");
		if (data->philos)
			free(data->philos);
		return (1);
	}
	init_philos(data);
	return (0);
}

static int	create_threads(t_data *data, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]))
		{
			printf("Error: thread creation failed\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_routine, data))
	{
		printf("Error: monitor thread creation failed\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor;
	int			i;

	if (init_simulation_data(&data, argc, argv))
		return (1);
	if (create_threads(&data, &monitor))
	{
		cleanup(&data);
		return (1);
	}
	i = 0;
	while (i < data.num_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	cleanup(&data);
	return (0);
}
