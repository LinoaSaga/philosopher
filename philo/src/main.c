/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 15:00:00 by ljudd             #+#    #+#             */
/*   Updated: 2026/04/14 15:00:00 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	if (parse_args(&data, ac, av))
		return (1);
	if (init_data(&data))
		return (1);
	monitor_philos(&data);
	cleanup(&data);
	return (0);
}
