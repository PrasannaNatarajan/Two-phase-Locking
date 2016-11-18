# Two-phase-Locking

# Synopsis

This is an implementation of two phase locking algorithm to solve the concurrency issues in the dinning philosophers problem in C.
pthreads are used to simulate the philosophers. Semaphors are used to lock the chopsticks and avoid concurrency issues. 

Dead lock prevention algorithm (pseudo code):

	acquire mutex lock
	check whether left and right chopsticks are free
	pick chopsticks up/acquire their semaphores/wait
	release mutex lock

# Compilation

  `gcc twoPhaseLock.c -o twoPhaseLock -lpthread`
  
# To run
  
  `./twoPhaseLock <number_of_philosophosers> <type_of_print>`
  
First parameter is the number of philosophers - default = 5  
Second parameter is the type of print  - default = 1
  
  0	-	No Print  
  1	-	Print Verbose  
  2	-	Print only current status of all philosophers  

# Contributors

Atish Majumdar,
<a href = "https://github.com/Prasannacool">Prasanna Natarajan</a>,
<a href = "https://github.com/vedantcj/">Vedant Chakravarthy</a>.
