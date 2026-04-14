/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lludd <lludd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:00:00 by lludd             #+#    #+#             */
/*   Updated: 2026/04/14 15:00:00 by lludd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_number(char *str)
{
	int	k;

	k = -1;
	if (!str || !str[0])
		return (0);
	while (str[++k])
	{
		if (str[k] < '0' || str[k] > '9')
			return (0);
	}
	return (1);
}

static long	ft_atol(char *str)
{
	long	result;
	int		k;

	result = 0;
	k = -1;
	while (str[++k])
	{
		result = result * 10 + (str[k] - '0');
		if (result > 2147483647)
			return (-1);
	}
	return (result);
}

static int	check_values(t_data *data, int ac)
{
	if (data->nb_philos <= 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0
		|| (ac == 6 && data->must_eat <= 0))
		return (1);
	return (0);
}

int	parse_args(t_data *data, int ac, char **av)
{
	int	k;

	k = 0;
	while (++k < ac)
	{
		if (!is_valid_number(av[k]))
		{
			write(2, "Error: invalid argument\n", 24);
			return (1);
		}
	}
	data->nb_philos = (int)ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	data->must_eat = -1;
	if (ac == 6)
		data->must_eat = (int)ft_atol(av[5]);
	if (check_values(data, ac))
	{
		write(2, "Error: invalid argument value\n", 30);
		return (1);
	}
	return (0);
}
