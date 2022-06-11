/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: net-touz <net-touz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 23:20:43 by net-touz          #+#    #+#             */
/*   Updated: 2022/06/09 10:07:36 by net-touz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
#include <sys/time.h>
typedef long long t_ll;
typedef unsigned long long t_ull;
//define colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"
//define states
# define THINKING 0
# define EATING 1
# define SLEEPING 2
# define DEAD 3
//define time
# define SECOND 1000000
# define MINUTE 60 * SECOND
# define HOUR 60 * MINUTE
# define DAY 24 * HOUR
# define YEAR 365 * DAY
//define forks
# define LEFT 0
# define RIGHT 1
//define error
# define ERR_THRD_CREATE "Fail to create thread"
# define ERR_THRD_JOIN "Fail to join thread"
# define ERR_MALLOC "Fail to malloc"
# define ERR_ARGGS "Wrong number of arguments"
# define ERR_ARG_VAL "Wrong argument value"
//Max number
# define MAX_PHILO 200
# define PHILO_SLEEPING_TIME 60
# define PHILO_EATING_TIME 60


typedef struct s_philo
{
	struct s_env	*env;
	int				id;
	pthread_t	thread;
	char			status;
	unsigned		dead_time;
	unsigned		eat_count;
	t_ull			last_meal_time;
	pthread_mutex_t	eat_mutex;
}					t_philo;

typedef struct 			s_env
{
	char				dead;
	pthread_mutex_t		*forks;
	pthread_mutex_t		logger;
	t_philo				*philos;
	t_ull				start_time;
	long				philo_count;
	long				time_to_eat;
	t_ull				time_to_die;
	long				time_to_sleep;
	long				meal_time;
	long				max_meals;
}						t_env;
void eating(t_philo *philo);
void take_forks(t_philo *philo);
void put_forks(t_philo *philo);
void thinking(t_philo *philo);
void sleeping(t_philo *philo);
void *philo_thread(void *arg);
t_ull get_time(void);
int start_simulation(t_env *env);
int parse_args(t_env *env, int argc, char **argv);
#endif