/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraynald <eraynald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 16:54:07 by eraynald          #+#    #+#             */
/*   Updated: 2022/04/27 16:54:07 by eraynald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//проверяем что ниодин из философов не умер, проверяем сколько раз
//должен поесть философ, после чего блокируем правую и левую вилку,
//о чем выводим сообщение, филосов ест, мы записываем последнее время еды,
//разблокируем вилки, после чего филосов идет спать и думать
void	live(t_philo *philo)
{
	while (!philo->maind->die
		&& philo->how_phil_eat != philo->maind->t_must_eat)
	{
		pthread_mutex_lock(&philo->maind->fork[philo->r_fork]);
		pthread_mutex_lock(&philo->maind->fork[philo->l_fork]);
		printf("%lu %d has taken a fork\n", get_time() - \
			philo->maind->start_time, philo->id);
		printf("%lu %d has taken a fork\n", get_time() - \
			philo->maind->start_time, philo->id);
		printf("%lu %d is eating\n", get_time() - \
			philo->maind->start_time, philo->id);
		usleep_control(philo->maind->t_t_eat);
		philo->how_phil_eat++;
		philo->t_last_eat = get_time();
		pthread_mutex_unlock(&philo->maind->fork[philo->r_fork]);
		pthread_mutex_unlock(&philo->maind->fork[philo->l_fork]);
		if (philo->how_phil_eat != philo->maind->t_must_eat)
		{
			printf("%lu %d is sleeping\n", get_time() - \
				philo->maind->start_time, philo->id);
			usleep_control(philo->maind->t_t_sleep);
			printf("%lu %d is thinking\n", get_time() - \
				philo->maind->start_time, philo->id);
		}
	}
}

int	check_death(t_maind *mdata, unsigned long now_t, int i, int f)
{
	while (1)
	{
		while (i < mdata->n_of_philo)
		{
			now_t = get_time();
			if (mdata->philo[i].how_phil_eat == mdata->t_must_eat)
				f++;
			if (f == mdata->n_of_philo)
				return (1);
			if (now_t - mdata->philo[i].t_last_eat > \
				(unsigned long)mdata->t_t_die)
			{
				mdata->die = 1;
				printf("\33[5m\33[41m%lu %d died\033[0m\n",
					now_t - mdata->start_time, i + 1);
				return (1);
			}
			i++;
		}
		i = 0;
		usleep(1000);
	}
	return (0);
}

void	thread_create(t_maind	*mdata)
{
	int	i;

	i = 0;
	while (i < mdata->n_of_philo)
	{
		pthread_create(&mdata->philo[i].thread, NULL,
			(void*)live, &mdata->philo[i]);
		usleep(100);
		i += 2;
	}
	i = 1;
	while (i < mdata->n_of_philo)
	{
		pthread_create(&mdata->philo[i].thread, NULL,
			(void*)live, &mdata->philo[i]);
		usleep(100);
		i += 2;
	}
	if (check_death(mdata, 0, 0, 0))
		return ;
	i = -1;
	while (++i < mdata->n_of_philo)
		pthread_join(mdata->philo[i].thread, NULL);
}

int	main(int argc, char *argv[])
{
	t_maind	mdata;
	int		i;

	if (parser(&mdata, argv, argc))
		return (error_alert(":wrong number of parameters"));
	if (init_fork_philo(&mdata))
		return (error_alert(":can't init fork"));
	mdata.start_time = get_time();
	thread_create(&mdata);
	free(mdata.fork);
	free(mdata.philo);
	i = -1;
	while (++i < mdata.n_of_philo)
		pthread_mutex_destroy(&mdata.fork[i]);
	return (0);
}
