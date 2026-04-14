/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lludd <lludd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:00:00 by lludd             #+#    #+#             */
/*   Updated: 2026/04/14 15:00:00 by lludd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_dead(t_data *data, int k)
{
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	printf("%ld %d died\n", get_time() - data->start_time,
		data->philos[k].id);
	pthread_mutex_unlock(&data->dead_mutex);
	pthread_mutex_unlock(&data->print_mutex);
}

static int	check_death(t_data *data)
{
	int		k;
	long	elapsed;

	k = -1;
	while (++k < data->nb_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		elapsed = get_time() - data->philos[k].last_meal_time;
		pthread_mutex_unlock(&data->meal_mutex);
		if (elapsed > data->time_to_die)
		{
			set_dead(data, k);
			return (1);
		}
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	k;
	int	finished;

	if (data->must_eat == -1)
		return (0);
	finished = 0;
	k = -1;
	while (++k < data->nb_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[k].meals_eaten >= data->must_eat)
			finished++;
		pthread_mutex_unlock(&data->meal_mutex);
	}
	if (finished == data->nb_philos)
	{
		pthread_mutex_lock(&data->dead_mutex);
		data->dead = 1;
		pthread_mutex_unlock(&data->dead_mutex);
		return (1);
	}
	return (0);
}

void	monitor_philos(t_data *data)
{
	int	k;

	data->start_time = get_time();
	k = -1;
	while (++k < data->nb_philos)
	{
		data->philos[k].last_meal_time = data->start_time;
		pthread_create(&data->philos[k].thread, NULL,
			philo_routine, &data->philos[k]);
	}
	while (1)
	{
		if (check_death(data) || check_all_ate(data))
			break ;
		usleep(500);
	}
	k = -1;
	while (++k < data->nb_philos)
		pthread_join(data->philos[k].thread, NULL);
}
