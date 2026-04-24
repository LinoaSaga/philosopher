*This project has been created as part of the 42 curriculum by ljudd.*

## Description

Philosophers is a concurrency project that simulates the classic **Dining Philosophers Problem**. A given number of philosophers sit around a circular table, alternating between eating, sleeping, and thinking. Each philosopher needs two forks to eat, but there is only one fork between each pair of neighbors. The challenge is to design a solution where no philosopher starves and data races are avoided.

This implementation uses **POSIX threads** (one per philosopher) and **mutexes** (one per fork, plus dedicated mutexes for printing, death checking, and meal tracking) to synchronize access to shared resources. An even/odd fork-pickup order prevents deadlocks, and an adaptive think time is applied for odd numbers of philosophers to ensure fair scheduling.

## Instructions

### Compilation

```bash
cd philo
make        # builds the executable
make clean  # removes object files
make fclean # removes object files and the executable
make re     # full recompile
```

### Execution

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` (ms) | Time after last meal before a philosopher dies |
| `time_to_eat` (ms) | Duration of eating |
| `time_to_sleep` (ms) | Duration of sleeping |
| `number_of_times_each_philosopher_must_eat` | *(optional)* Simulation stops when every philosopher has eaten at least this many times |

### Examples

```bash
./philo 5 800 200 200        # 5 philosophers, nobody should die
./philo 4 410 200 200        # 4 philosophers, nobody should die
./philo 1 800 200 200        # 1 philosopher, should die at 800ms
./philo 5 800 200 200 7      # stops after each philosopher has eaten 7 times
```

## Resources

- [The Dining Philosophers Problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- `man pthread_create`, `man pthread_mutex_lock`, `man gettimeofday`
- [42 Philosophers guide by nafMusic](https://github.com/nafMusic/philosophers-42)
- AI (Claude) was used as a development aid throughout this project: understanding the subject requirements, debugging concurrency issues, reviewing thread safety, and generating this README. All code was reviewed, tested, and validated manually.
