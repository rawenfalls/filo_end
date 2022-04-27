/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_dop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraynald <eraynald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 17:08:17 by eraynald          #+#    #+#             */
/*   Updated: 2022/04/27 17:11:58 by eraynald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Сообщение об ошибке выводит ERROR который
//горит красным и мигает, а так же строку, возвращяет 1.
int	error_alert(char *str)
{
	printf ("\33[5m\33[41mERROR\033[0m %s\n", str);
	return (1);
}

//Возвращает преобразованную строку в число, дополнительных проверок не делает
int	ft_atoi(char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	if (*str == '-')
		sign *= -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
		res = res * 10 + (*str++ - '0');
	return ((int)(res * sign));
}

//проверяет ввод на соответствие параметров, а так же заполняет структуру
int	parser(t_maind	*mdata, char **argv, int argc)
{
	if (argc != 5 && argc != 6)
		return (1);
	mdata->n_of_philo = ft_atoi(argv[1]);
	mdata->t_t_die = ft_atoi(argv[2]);
	mdata->t_t_eat = ft_atoi(argv[3]);
	mdata->t_t_sleep = ft_atoi(argv[4]);
	mdata->die = 0;
	mdata->t_must_eat = -1;
	if (argc == 6)
		mdata->t_must_eat = ft_atoi(argv[5]);
	return (0);
}

//инициализируем философов
void	init_philo(t_maind *mdata, int i)
{
	mdata->philo[i].id = i + 1;
	mdata->philo[i].r_fork = i;
	mdata->philo[i].maind = mdata;
	mdata->philo[i].how_phil_eat = 0;
	if (i == 0)
		mdata->philo[i].l_fork = mdata->n_of_philo - 1;
	else
		mdata->philo[i].l_fork = i - 1;
	mdata->philo[i].t_last_eat = get_time();
}

//выделяем память для философов и вилок, инициализируем мютексы,
// запускаем функцию для инициализации философов
int	init_fork_philo(t_maind *mdata)
{
	int	i;

	mdata->philo = malloc(sizeof(t_philo) * mdata->n_of_philo);
	if (!mdata->philo)
		return (error_alert("can't malloc memory on philo"));
	mdata->fork = malloc(sizeof(pthread_mutex_t) * mdata->n_of_philo);
	if (!mdata->fork)
	{
		free(mdata->philo);
		return (error_alert("can't malloc memory on fork"));
	}
	i = -1;
	while (++i < mdata->n_of_philo)
	{
		pthread_mutex_init(&mdata->fork[i], NULL);
		init_philo(mdata, i);
	}
	return (0);
}
