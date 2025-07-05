/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:46:38 by aybelaou          #+#    #+#             */
/*   Updated: 2025/07/05 14:41:58 by aybelaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philosopher.h
 * @brief Header file for the Dining Philosophers simulation project
 * 
 * This file contains all structure definitions, function prototypes,
 * and constants needed for the philosopher simulation. The project
 * implements the classic dining philosophers problem using threads
 * and mutexes to demonstrate concurrency and synchronization.
 * 
 * @author aybelaou
 * @date 2025-07-04
 * @version 1.0
 */

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

/* Color codes for terminal output */
# define RED "\033[31m"
# define RS "\033[0m"
# define CY "\033[36m"
# define YL "\033[33m"
# define GR "\033[32m"
# define MG "\033[35m"

/**
 * @struct s_philo
 * @brief Structure representing a single philosopher
 * 
 * Contains all data needed for a philosopher thread including
 * identification, meal tracking, thread handle, and fork pointers.
 */
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

/**
 * @struct s_data
 * @brief Structure containing all shared simulation data
 * 
 * This structure holds all parameters, timing information, philosophers,
 * mutexes, and synchronization data needed for the simulation.
 */
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

/* ========================= CORE FUNCTIONS ========================= */

/**
 * @brief Main philosopher thread routine
 * @param arg Pointer to philosopher structure (t_philo*)
 * @return NULL when thread exits
 * 
 * Main loop for each philosopher: eat, sleep, think cycle.
 * Implements even/odd desynchronization for optimal resource usage.
 */
void		*philosopher_routine(void *arg);

/**
 * @brief Monitor thread that checks for deaths and completion
 * @param data Pointer to shared simulation data
 * 
 * Continuously monitors all philosophers for death conditions
 * and checks if all have eaten the required number of meals.
 */
void		monitor_philosophers(t_data *data);

/* ========================= INITIALIZATION ========================= */

/**
 * @brief Initialize all mutex objects
 * @param data Pointer to simulation data structure
 * @return true on success, false on failure
 * 
 * Creates and initializes all mutexes with proper error handling
 * and cleanup on failure.
 */
bool		init_mutexes(t_data *data);

/**
 * @brief Initialize simulation data structure
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Pointer to initialized data structure, NULL on failure
 * 
 * Main initialization function that sets up all simulation data.
 */
t_data		*init_data(int argc, char **argv);

/**
 * @brief Parse and validate command line arguments
 * @param data Pointer to data structure to fill
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return true if arguments are valid, false otherwise
 */
bool		parse_args(t_data *data, int argc, char **argv);

/**
 * @brief Initialize philosopher structures and assign forks
 * @param data Pointer to simulation data
 * @return true on success, false on failure
 */
bool		init_philosophers(t_data *data);

/* ========================= THREAD MANAGEMENT ========================= */

/**
 * @brief Create all philosopher threads
 * @param data Pointer to simulation data
 * @return true if all threads created successfully, false otherwise
 */
bool		create_philosophers(t_data *data);

/**
 * @brief Wait for all philosopher threads to complete
 * @param data Pointer to simulation data
 */
void		join_philosophers(t_data *data);

/* ========================= UTILITIES ========================= */

/**
 * @brief Get current time in milliseconds
 * @return Current timestamp in milliseconds since epoch
 */
long long	get_time(void);

/**
 * @brief High-precision sleep function
 * @param duration Sleep duration in milliseconds
 * 
 * Uses busy-waiting for precise timing control.
 */
void		precise_sleep(long long duration);

/**
 * @brief Check if simulation should end
 * @param data Pointer to simulation data
 * @return true if simulation should end, false otherwise
 * 
 * Thread-safe check of the simulation_end flag.
 */
bool		is_simulation_over(t_data *data);

/**
 * @brief Safe string to integer conversion
 * @param str String to convert
 * @param error Pointer to error flag (set to true on error)
 * @return Converted integer value, 0 on error
 */
int			ft_atoi_safe(const char *str, bool *error);

/**
 * @brief Calculate string length
 * @param str Input string
 * @return Length of string in characters
 */
int			ft_strlen(const char *str);

/* ========================= SIMULATION CONTROL ========================= */

/**
 * @brief Set simulation end flag safely
 * @param data Pointer to simulation data
 * @param value New value for simulation_end flag
 */
void		set_simulation_end(t_data *data, bool value);

/**
 * @brief Check if all philosophers have eaten enough
 * @param data Pointer to simulation data
 * @return true if all have eaten required meals, false otherwise
 */
bool		all_ate_enough(t_data *data);

/* ========================= PHILOSOPHER ACTIONS ========================= */

/**
 * @brief Acquire both forks for eating
 * @param philo Pointer to philosopher structure
 * 
 * Implements deadlock prevention through even/odd fork ordering.
 */
void		take_forks(t_philo *philo);

/**
 * @brief Execute eating action
 * @param philo Pointer to philosopher structure
 * 
 * Takes forks, eats for specified duration, updates meal data,
 * and releases forks.
 */
void		eat(t_philo *philo);

/* ========================= OUTPUT FUNCTIONS ========================= */

/**
 * @brief Thread-safe status printing with timestamps
 * @param philo Pointer to philosopher structure
 * @param status Status message to print
 * @param color Color code for the message (can be NULL)
 * 
 * Optimized printing function using single write() call for
 * precise timing and clean output.
 */
void		ft_print_status(t_philo *philo, char *status, char *color);

/**
 * @brief Print string to stdout
 * @param str String to print
 */
void		ft_putstr(const char *str);

/* ========================= INPUT VALIDATION ========================= */

/**
 * @brief Validate command line input
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return true if input is valid, false otherwise
 */
bool		validate_input(int argc, char **argv);

/* ========================= CLEANUP ========================= */

/**
 * @brief Clean up all allocated resources
 * @param data Pointer to simulation data
 * 
 * Destroys all mutexes and frees all allocated memory.
 */
void		clean_up(t_data *data);

#endif
