/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:53:00 by vpoelman          #+#    #+#             */
/*   Updated: 2025/11/23 23:53:00 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	cleanup_forks(t_data *data)
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
}

void	cleanup_mutexes(t_data *data)
{
	cleanup_forks(data);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
