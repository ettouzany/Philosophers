/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: net-touz <net-touz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 20:34:37 by net-touz          #+#    #+#             */
/*   Updated: 2022/06/09 11:01:48 by net-touz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
// # define ERR_THRD_CREATE "Fail to create thread"
// # define ERR_THRD_JOIN "Fail to join thread"
// # define ERR_MALLOC "Fail to malloc"
// # define ERR_ARGGS "Wrong number of arguments"
// # define ERR_ARG_VAL "Wrong argument value"
// # define MAX_PHILO 200
// # define MAX_PHILO_SLEEPING_TIME 60
// # define MAX_PHILO_EATING_TIME 60
int ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
int watch_threads(t_env *env)
{
	int i;

	while (1)
	{
		i = 0;
		while (i < env->philo_count)
		{
			pthread_mutex_lock(&env->philos[i].eat_mutex);
			if(get_time() - env->philos[i].last_meal_time >= env->time_to_die && env->philos[i].status == THINKING)
			{
				env->philos[i].status = DEAD;
				env->dead = 1;
				printf("%016llu  %d is Dead\n", get_time()- env->start_time, env->philos[i].id);
				return(1);
			}
			pthread_mutex_unlock(&env->philos[i].eat_mutex);
		}
		usleep(100);
	}
	return (0);
}
int ft_atoi(char *str)
{
	int i;
	int nb;

	i = 0;
	nb = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		return (-ft_atoi(&str[i + 1]));
	if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb);
}
int main(int argc, char **argv)

{
	t_env	*env;

	
	if (argc != 5 && argc != 6)
		return (printf(ERR_ARGGS));
	env = (t_env*)malloc(sizeof(t_env));
	if (!env)
		return (printf(ERR_MALLOC), 0);	
	if (parse_args(env, argc, argv))
		return (printf(ERR_ARG_VAL));
	env->dead = 0;

	if (start_simulation(env))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int check_arg(char *arg)
{
	unsigned i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (ft_atoi(arg));
}

int parse_args(t_env *env, int argc, char **argv)
{
	env->philo_count = check_arg(argv[1]);
	env->time_to_die = check_arg(argv[2]);
	env->time_to_eat = check_arg(argv[3]);
	env->time_to_sleep = check_arg(argv[4]);
	if (argc == 6)
		env->max_meals = check_arg(argv[5]);
	else
		env->max_meals = -1;
	if (env->philo_count < 2 || env->philo_count > MAX_PHILO)
		return (1);
	if (env->time_to_die <  0)
		return (1);
	if (env->time_to_eat < PHILO_EATING_TIME)
		return (1);
	if (env->time_to_sleep < PHILO_SLEEPING_TIME)
		return (1);
	if (env->max_meals < 0 && argc == 6)
		return (1);
	return (0);
}
					
int start_simulation(t_env *env)
{
	int i;

	i = 0;
	env->start_time = get_time();
	env->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * env->philo_count);
	env->philos = (t_philo*)malloc(sizeof(t_philo) * env->philo_count);
	if (!env->forks || !env->philos)
		return (1);
	while (i < env->philo_count)
	{
		pthread_mutex_init(&env->forks[i], NULL);
		env->philos[i].id = i;
		env->philos[i].env = env;
		env->philos[i].status = THINKING;
		env->philos[i].eat_count = 0;
		i++;
	}
	i = 0;
	while (i < env->philo_count)
	{
		if(i%2 == 0)
			if(pthread_create(&env->philos[i].thread, NULL, &philo_thread, &env->philos[i]))
				return (1);
		i++;
	}
	usleep(400);
	i = 0;
	while (i < env->philo_count)
	{
		if(i%2 == 1)
			if(pthread_create(&env->philos[i].thread, NULL, &philo_thread, &env->philos[i]))
				return (1);
		i++;
	}
	if(watch_threads(env))
		{
			 
		}
	return (0);
}

t_ull get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void *philo_thread(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo->env->max_meals == -1 || philo->eat_count < philo->env->max_meals)
	{
		thinking(philo);
		take_forks(philo);
		eating(philo);
		put_forks(philo);
		sleeping(philo);
	}
	return (NULL);
}

void thinking(t_philo *philo)
{
	philo->last_meal_time = get_time();
	printf("%016llu  %d is thinking\n", get_time()-philo->env->start_time, philo->id);
	philo->status = THINKING;
}

void take_forks(t_philo *philo)
{
	//check if forks are available
	pthread_mutex_lock(&philo->env->forks[(philo->id + LEFT) % philo->env->philo_count]);
	pthread_mutex_lock(&philo->env->forks[(philo->id + RIGHT) % philo->env->philo_count]);
	pthread_mutex_lock(&philo->eat_mutex);
	if(philo->env->dead)
	{
		pthread_mutex_unlock(&philo->eat_mutex);
		pthread_mutex_unlock(&philo->env->forks[(philo->id + RIGHT) % philo->env->philo_count]);
		pthread_mutex_unlock(&philo->env->forks[(philo->id + LEFT) % philo->env->philo_count]);
		return;
	}
	printf("%016llu  %d is taking forks\n", get_time()-philo->env->start_time, philo->id);
}

void eating(t_philo *philo)
{
	philo->status = EATING;
	printf("%016llu  %d is eating\n", get_time()-philo->env->start_time, philo->id);
	usleep(philo->env->time_to_eat);
	philo->eat_count++;
	philo->last_meal_time = get_time();
}

void put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->env->forks[philo->id]);
	pthread_mutex_unlock(&philo->env->forks[(philo->id + 1) % philo->env->philo_count]);
	pthread_mutex_unlock(&philo->eat_mutex);
	printf("%016llu  %d is putting forks\n", get_time()-philo->env->start_time, philo->id);
}

void sleeping(t_philo *philo)
{
	philo->status = SLEEPING;
	printf("%016llu  %d is sleeping\n", get_time() - philo->env->start_time, philo->id);
	usleep(philo->env->time_to_sleep);
	philo->last_meal_time = get_time();
}

void print_status(t_env *env)
{
	int i;

	i = 0;
	while (i < env->philo_count)
	{
		printf("%d ", env->philos[i].status);
		i++;
	}
	printf("\n");
}
