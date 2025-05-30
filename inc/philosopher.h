/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:46:38 by aybelaou          #+#    #+#             */
/*   Updated: 2025/05/30 17:00:08 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

# define RED "\033[31m"
# define RS "\033[0m"
# define CY "\033[36m"
# define YL "\033[33m"
# define GR "\033[32m"
# define MG "\033[35m"

typedef struct s_philo
{
    int				id;
    int				meals_eaten;
    long long		last_meal_time;
    pthread_t		thread;
    pthread_mutex_t	*left_fork;
    pthread_mutex_t	*right_fork;
    struct s_data	*data;
}	t_philo;

typedef struct s_data
{
    int				num_philos;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				must_eat_count;
    bool			simulation_end;
    long long		start_time;
    t_philo			*philos;
    pthread_mutex_t	*forks;
    pthread_mutex_t	print_mutex;
    pthread_mutex_t	end_mutex;
	pthread_mutex_t	meal_mutex;
}	t_data;

// Function declarations
void		*philosopher_routine(void *arg);
void		monitor_philosophers(t_data *data);
t_data		*init_data(int argc, char **argv);
void		clean_up(t_data *data);
long long	get_time(void);
void		precise_sleep(long long duration);
void		print_status(t_philo *philo, char *status, char *color);
bool		is_simulation_over(t_data *data);
int			ft_atoi_safe(const char *str, bool *error);
void		set_simulation_end(t_data *data, bool value);
bool		all_ate_enough(t_data *data);
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);

#endif