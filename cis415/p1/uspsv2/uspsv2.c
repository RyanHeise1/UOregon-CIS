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
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "p1fxns.h"

#define UNUSED __attribute__((unused))

typedef struct MyObject{
	char *program;
	char **args;
	pid_t pid;
}MyObject;

void purge(struct MyObject *array, int line_count){
	for (int i = 0; i < line_count; i++){
		free(array[i].program);
		int k = 0;
		while (array[i].args[k] != NULL){
			free(array[i].args[k]);
			k++;
		}
		free(array[i].args);
		
	}
}

void execute(struct MyObject *array, int numprograms){
	int status;
	char *tmp[100];
	int recieved;
	int i;

	// create set of signals
	sigset_t signals;
	// initialize it to an empty set
	sigemptyset(&signals);
	// add signals of interest
	sigaddset(&signals, SIGUSR1);
	sigaddset(&signals, SIGCONT);
	sigaddset(&signals, SIGSTOP);
	// hold off normal delivery of signals
	int proc_mask = sigprocmask(SIG_BLOCK, &signals, NULL);
	// catch error with sigprocmask
	if (proc_mask != 0){
		p1perror(2, "ERROR while creating sigprocmask()!");
	}

	for (i = 0; i < numprograms; i++){
		int j = 0;
		int k = 1;
		char *program = array[i].program;

		// fill 1-d array with program name and arguments
		tmp[0] = program; // first index of tmp is always program name
		while (array[i].args[j] != NULL){ // loop over struct untill you reach null
			tmp[k++] = p1strdup(array[i].args[j]); // add argument to current index of tmp
			j++;
		}
		tmp[j + 1] = NULL;

		array[i].pid = fork();
		if (array[i].pid == 0){
			status = sigwait(&signals, &recieved);
			if (status != 0){
				p1perror(2, "ERROR with sigwait()!");
			}
			if (execvp(tmp[0], tmp) < 0){ // avoid fork bombs
				exit(1);
			}
		}
		// clear array[] for next line
		k = 0;
		tmp[k++] = NULL;
		while(tmp[k] != NULL){
			free(tmp[k]); // because I used strdup
			k++;
		}
		// waitpid(array[i].pid, &status, 0);
	}
	// status = sigprocmask(SIG_UNBLOCK, &signals, NULL);

	for (i = 0; i < numprograms; i++){
		kill(array[i].pid, SIGUSR1);
	}
	for (i = 0; i < numprograms; i++){
		kill(array[i].pid, SIGSTOP);
	}
	for (i = 0; i < numprograms; i++){
		kill(array[i].pid, SIGCONT);
	}

	for (i = 0; i < numprograms; i++){
		waitpid(array[i].pid, &status, 0);
	}
}

int count_words(char *buf){
	int index = 0;
	int word_count = 0;
	char word[BUFSIZ];

	// Count number of words in line (used to malloc enough space)
	while ((index = p1getword(buf, index, word)) != -1){
		word_count++;
	}
	return word_count;
}

void read_file(int fd, struct MyObject *array){
	char buf[BUFSIZ];
	char word[BUFSIZ];
	int line_count = 0;
	int index = 0;
	int i = 0;
	int size = 0;
	
	// Read lines from fd into buf
	while((size = p1getline(fd, buf, BUFSIZ)) != 0){
		line_count += 1;
		if(buf[size - 1] == '\n'){
			buf[size - 1] = '\0';
		}
		// first word of each line is going to be the program
		index = p1getword(buf, index, word);
		array[i].program = p1strdup(word);


		// find number of words in the string
		int word_count = count_words(buf);
		// malloc MyObject.args with the number of words
		array[i].args = malloc(word_count * sizeof(array[i].args));

		// loop through each word in the line
		int arg_count = 0;
		while ((index = p1getword(buf, index, word)) != -1){
			array[i].args[arg_count] = p1strdup(word);
			arg_count += 1;
		}
		// insert null to the end of the args list
		array[i].args[arg_count] = NULL;
		// increment index of array
		i++;
		// set index of the word to 0 so we can process next line
		index = 0;
	}

	execute(array, line_count);
	purge(array, line_count);
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

	// create array of structs
	struct MyObject array[100];

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
		// TODO: Do I need to overide the USPS_QUANTUM_MSEC variable?
		qValue = p1atoi(q_char);
	}else{
		p1perror(2, "Error finding USPS_QUANTUM_MSEC variable\n");
		return EXIT_FAILURE;
	}

	// process cmd line entry
	if (argv[optind] == NULL){ //No file specified. Processing stdin
		read_file(0, array);
		return EXIT_SUCCESS;
	}else { // file present
		filename = argv[optind];
		int fd = open(filename, O_RDONLY); // open file for read only
		if (fd != -1){ // File exits
			read_file(fd, array);
		}else{
			p1perror(2, "Error: filename does not exist\n");
			return EXIT_FAILURE;
		}
		close(fd);
	}
}
