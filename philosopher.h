/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpoelman <vpoelman@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:12:34 by vpoelman          #+#    #+#             */
/*   Updated: 2025/09/11 15:12:34 by vpoelman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				finished;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	int				is_dead;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}					t_data;

// utils.c
int					ft_atoi(const char *str);
long long			get_time(void);
void				print_status(t_philo *philo, char *status);
void				ft_usleep(long long time);
void				cleanup_mutexes(t_data *data);
void				cleanup_forks(t_data *data);

// init.c
int					init_data(t_data *data, int argc, char **argv);
int					init_philos(t_data *data);
int					init_mutexes(t_data *data);

// philo_actions.c
void				release_forks(t_philo *philo);
void				eat(t_philo *philo);

// philo_fork_utils.c
int					take_forks(t_philo *philo);

// routine.c
void				*philo_routine(void *arg);
void				*monitor_routine(void *arg);

// main.c
int					main(int argc, char **argv);

#endif