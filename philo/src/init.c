/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:00:00 by ljudd             #+#    #+#             */
/*   Updated: 2026/04/14 15:00:00 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_forks(t_data *data)
{
	int	k;

	data->forks = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	k = -1;
	while (++k < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[k], NULL))
			return (1);
	}
	return (0);
}

static int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->dead_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->meal_mutex, NULL))
		return (1);
	return (0);
}

static int	init_philos(t_data *data)
{
	int	k;

	data->philos = (t_philo *)malloc(
			sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (1);
	k = -1;
	while (++k < data->nb_philos)
	{
		data->philos[k].id = k + 1;
		data->philos[k].meals_eaten = 0;
		data->philos[k].last_meal_time = data->start_time;
		data->philos[k].left_fork = &data->forks[k];
		data->philos[k].right_fork
			= &data->forks[(k + 1) % data->nb_philos];
		data->philos[k].data = data;
	}
	return (0);
}

int	init_data(t_data *data)
{
	data->dead = 0;
	data->start_time = get_time();
	if (init_forks(data))
		return (1);
	if (init_mutexes(data))
		return (1);
	if (init_philos(data))
		return (1);
	return (0);
}
