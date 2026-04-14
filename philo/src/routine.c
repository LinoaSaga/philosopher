/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:00:00 by ljudd             #+#    #+#             */
/*   Updated: 2026/04/14 15:00:00 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

static void	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	print_status(philo, "is eating");
	smart_sleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos % 2 == 0)
		return ;
	think_time = philo->data->time_to_eat * 2
		- philo->data->time_to_sleep;
	if (think_time < 0)
		think_time = 0;
	smart_sleep(think_time, philo->data);
}

static void	*lone_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	smart_sleep(philo->data->time_to_die + 1, philo->data);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philos == 1)
		return (lone_philo(philo));
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 500);
	while (!is_dead(philo->data))
	{
		eat(philo);
		if (is_dead(philo->data))
			break ;
		print_status(philo, "is sleeping");
		smart_sleep(philo->data->time_to_sleep, philo->data);
		think(philo);
	}
	return (NULL);
}
