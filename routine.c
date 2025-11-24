/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:12:38 by vpoelman          #+#    #+#             */
/*   Updated: 2025/11/06 18:49:56 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->is_dead || philo->finished)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (take_forks(philo))
			break ;
		eat(philo);
		release_forks(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}
