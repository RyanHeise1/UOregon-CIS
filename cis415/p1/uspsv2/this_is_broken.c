/*
 USAGE:
 	./uspsv2 [-q <quantun in msec>] [workload_file]

 1. Need to implement a way for the USPS to stop all processes 
 	just before they call execvp() so the USPS can decide which 
 	process to run first
 		a. Have each forked child process wait for a SIGURSR1 
 		   signal before calling execvp() (Hint: Use sigwait())
 		b. USPS parent process sends the SIGUSR1 signal to the 
 		   corresponding forked child process
 2. Need to implent a mechanism for the USPS to signal a running 
 	process to stop (SIGSTOP signal) and to continue it again
 	(SIGCONT signal).


 
 o. Immediately after each process is created using fork(), the
 	child process waits on the SIGUSR1 signal before calling 
 	execvp()
 o. After ALL of the processes have been created and are awaiting
 	the SIGUSR1 signal, the USPS parent process sends each program
 	a SIGUSR1 signal to wake them up. Each child process will then
 	invoke execvp()
 o. After ALL of the processes have been awakened and are executing, 
 	the USPS sends each process a SIGSTOP signal to suspend it
 o. After ALL of the workload processes have been suspended, the 
 	USPS sends each process a SIGCONT signal to resume it. 
 o. Once all processes are back up and running, the USPS waits for
 	each process to terminate. After all have terminated, USPS exits
*/

#include <stdio.h> // BUFSIZ
#include <stdlib.h>  //EXIT_SUCCESS
#include <stdbool.h> // bools
#include <unistd.h> // getopt and optind
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "p1fxns.h"

#define UNUSED __attribute__((unused))

void purge(char **args, int word_cnt){
	for (int i = 0; i < word_cnt; i++){
		free(args[i]);
	}
	free(args);
}

void exectute(char **args, int word_cnt){
	pid_t pid;
	int recieved;
	int i;
	int status;
	char *array[100];

	// create array for the arguments/ prog name
	for (i = 0; i < word_cnt; i++){
		array[i] = p1strdup(args[i]);
	}
	array[i] = NULL; // last elem in array needs to be null

	// create set of signals
	sigset_t signals;
	// initialize it to an empty set
	sigemptyset(&signals);
	// add signals of interest
	sigaddset(&signals, SIGUSR1);
	sigaddset(&signals, SIGCONT);
	sigaddset(&signals, SIGSTOP);
	// hold off normal delivery of signals (dont want normal behavior)
	int proc_mask = sigprocmask(SIG_BLOCK, &signals, NULL);
	// catch error with sigprocmask
	if (proc_mask != 0){
		p1perror(2, "ERROR while creating sigprocmask()!");
	}

	pid = fork();
	if (pid == 0){ // TODO Do I need to do something with the parent process
		status = sigwait(&signals, &recieved);
		// signal(SIGCONT, SIG_DFL);
		if (status != 0){
			p1perror(2, "ERROR with sigwait()!");
		}
		if (execvp(array[0], array) < 0){ // avoid fork bombs
			exit(1);
		}
	} // TODO Make an error else { ... } 

	// clear array[] for next line to be processed
	for (i = 0; i < word_cnt; i++){
		array[i] = NULL;
	}

	status = sigprocmask(SIG_UNBLOCK, &signals, NULL);
	kill(pid, SIGUSR1);
	kill(pid, SIGSTOP);
	kill(pid, SIGCONT);
	
	waitpid(pid, &status, 0);
	//p1putstr(1, "\n"); // used to separate commands on stdout
}

void parse_command(char buf[]){
	char *line = buf;
	char word[BUFSIZ];
	int word_cnt = 0;
	int ans = 0;
	int i = 0;

	// Count number of words in line (used to malloc enough space)
	while ((ans = p1getword(line, ans, word)) != -1){
		word_cnt++;
	}

	// allocate array to store prog name and args
	char **args = malloc((word_cnt) * sizeof(*args)); 
	for (i = 0; i < word_cnt; i++){
		args[i] = malloc((word_cnt) * sizeof(args));
	}
	if (args == NULL){
		p1perror(2, "ERROR allocating space for args!!!");
		exit(1);
	}

	// Insert words into args
	ans = 0;
	i = 0;
	while ((ans = p1getword(line, ans, word)) != -1){
		p1strcpy(args[i++], word);
	}

	exectute(args, word_cnt);

	// free array
	purge(args, word_cnt);
}

void read_stdin(){
	char buf[BUFSIZ];
	int size;

	while ((size = p1getline(0, buf, BUFSIZ)) != 0){
		if(buf[size - 1] == '\n'){
			buf[size - 1] = '\0';
		}
		parse_command(buf);
	}
}

void read_file(int fd){
	/*
	 * This function takes in a file descriptor (fd) and reads the
	 * lines into buf.
	 */
	char buf[BUFSIZ];
	int size;

	// Read lines from fd into buf
	while((size = p1getline(fd, buf, BUFSIZ)) != 0){
		if(buf[size - 1] == '\n'){
			buf[size - 1] = '\0';
		}
		parse_command(buf);
	}
}

int main(int argc, char **argv){
	/*
 	 * main function that catches flags and opens/closes file
	 */
	int opt;
	UNUSED int qValue;
	char *q_char = NULL;
	char *p;
	char *filename = "";
	bool ifQ;
	

	ifQ = false;
	while((opt = getopt(argc, argv, "q:")) != -1){
		switch(opt){
			case 'q': 
				ifQ = true; 
				q_char = optarg; 
				break;
			default:
				p1perror(2, "usage: ./uspsv1 [-q <quantun in msec>] [workload_file]\n");
				return EXIT_FAILURE;
		}
	}

	// catch if valid decimal after -q
	if (ifQ){
		if (p1atoi(q_char) == 0){
			// p1putstr()
			p1perror(2, "usage: ./uspsv1 [-q <quantun in msec>] [workload_file]\n");
			return EXIT_FAILURE;
		}
	}

	// find USPS_... variable
	if (((p = getenv("USPS_QUANTUM_MSEC")) != NULL) && !(ifQ)){
		qValue = p1atoi(p);
	}else if (ifQ){  // OVERIDE environment variable
		qValue = p1atoi(q_char);
	}else{
		p1perror(2, "Error finding USPS_QUANTUM_MSEC variable\n");
		return EXIT_FAILURE;
	}

	// process cmd line entry
	if (argv[optind] == NULL){ //No file specified. Processing stdin
		read_stdin();
		return EXIT_SUCCESS;
	}else { // file present
		filename = argv[optind];
		int fd = open(filename, O_RDONLY); // open file for read only
		if (fd != -1){ // File exits
			read_file(fd);
		}else{
			p1perror(2, "Error: filename does not exist\n");
			return EXIT_FAILURE;
		}
		close(fd);
	}
}
