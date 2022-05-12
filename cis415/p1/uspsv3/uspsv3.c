/*
 USAGE:
 	./uspsv2 [-q <quantun in msec>] [workload_file]

 1. Implement a scheduler that runs the processes according to some
 	scheduling policy
 		a) Simplest policy is to equally share the processors by giving
 		   each process the same amount of time to run
 2. After time slice completes, suspend the process and start up another
 	ready process
*/
#include "ADTs/queue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "p1fxns.h"

#define UNUSED __attribute__((unused))
#define MAX 128
#define MAXBUF 4096
#define MIN_QUANTUM 20
#define MAX_QUANTUM 2000
#define MS_PER_TICK 20

typedef struct MyObject{
	char *program;
	char **args;
	pid_t pid;
}MyObject;

typedef struct pcb {
	pid_t pid;
	int ticks;
	bool isalive;
	bool usr1;
} PCB;

sigset_t signals;
PCB pcb_array[MAX]; 
struct MyObject prog_array[100];
int num_procs = 0;

volatile int active_processes = 0;
volatile int usr1 = 0;
pid_t parent;
const Queue *q = NULL;
PCB *current = NULL;
int ticks_in_quantum = 0;

static int pid2index(pid_t pid) {
	int i;
	for(i = 0; i < num_procs; ++i) {
		if(pcb_array[i].pid == pid)
			return 	i;
	}
	return -1;
}

static void usr1_handler(UNUSED int sig) {
	usr1++;
}

static void usr2_handler(UNUSED int sig) {
	// Nothing here
}

static void chld_handler(UNUSED int sig) {
	pid_t pid;
	int tmp;
	int status;

	
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
		//status of the process is actually dead
		if (WIFEXITED(status) || WIFSIGNALED(status)){
			active_processes--;
			tmp = pid2index(pid);
			pcb_array[tmp].isalive = false;
			kill(parent, SIGUSR2);
		}
	}
		
}
static void alrm_handler(UNUSED int sig) {
	//if current is not null, AKA we have a process lined up
	if (current != NULL){
		// if it's alive:
		if (current->isalive){
			// we decrement the time
			--current->ticks;
			// if there is still time:
			if (current->ticks > 0){
				// it can still run. Return;
				return;
			}
			// stop the process
			kill(current->pid, SIGSTOP);
			// PRINT INFO HERE TODO
			// enqueue the process
			q->enqueue(q, ADT_VALUE(current));
		}
		current = NULL;
	}
	// while we can dequeue stuff: dequeue and set current to that
	while(q->dequeue(q, ADT_ADDRESS(&current))){
		// if current is not alive:
		if (!(current->isalive))
			continue;
		// set current time
		current->ticks = ticks_in_quantum;
		// if current has seen usr1
		if(current->usr1){
			current->usr1 = false;
			// call usr1 signal on process
			kill(current->pid, SIGUSR1);
		}else{
			// call sigcont on process
			kill(current->pid, SIGCONT);
		}
	return;
	}
}

void purge(struct MyObject *prog_array, int line_count){
	for (int i = 0; i < line_count; i++){
		free(prog_array[i].program);
		int k = 0;
		while (prog_array[i].args[k] != NULL){
			free(prog_array[i].args[k]);
			k++;
		}
		free(prog_array[i].args);
	}
}

void set_signals(){
	if (signal(SIGUSR1, usr1_handler) == SIG_ERR){
		p1perror(2, "Error creating SIGUSR1 signal");
		exit(1);
	}
	if (signal(SIGUSR2, usr2_handler) == SIG_ERR){
		p1perror(2, "Error creating SIGUSR2 signal");
		exit(1);
	}

	parent = getpid();

	if (signal(SIGALRM, alrm_handler) == SIG_ERR){
		p1perror(2, "Error creating SIGALRM signal");
		exit(1);
	}
	if (signal(SIGCHLD, chld_handler) == SIG_ERR){
		p1perror(2, "Error creating SIGCHLD signal");
		exit(1);
	}
}

void set_timer(int quantum){
	struct itimerval time;

	time.it_value.tv_sec = quantum / 1000;
	time.it_value.tv_usec = (quantum * 1000) % 1000000;
	time.it_interval = time.it_value;
	if (setitimer(ITIMER_REAL, &time, NULL) == -1){
		p1perror(2, "Error in set_timer()");
		for(int i = 0; i < num_procs; ++i)
			kill(pcb_array[i].pid, SIGKILL);
		exit(1);
	}
}

void pid_to_queue(pid_t pid, int index){
	PCB *p = pcb_array + index;
	p->pid = pid;
	p->isalive = true;
	p->usr1 = true;
	q->enqueue(q, ADT_VALUE(p));
}

void execute(struct MyObject *prog_array){
	char *tmp[100];
	int i;
	struct timespec ms20 = {0, 20000000};

	q = Queue_create(doNothing);
	for (i = 0; i < num_procs; i++){
		int j = 0;
		int k = 1;
		char *program = prog_array[i].program;

		// fill 1-d prog_array with program name and arguments
		tmp[0] = program; // first index of tmp is always program name
		while (prog_array[i].args[j] != NULL){ // loop over struct untill you reach null
			tmp[k++] = p1strdup(prog_array[i].args[j]); // add argument to current index of tmp
			j++;
		}
		tmp[j + 1] = NULL;

		prog_array[i].pid = fork();
		if (prog_array[i].pid == 0){
			while(!usr1)
				nanosleep(&ms20, NULL);

			if (execvp(tmp[0], tmp) < 0){ // avoid fork bombs
				exit(1);
			}
		}else{
			pid_to_queue(prog_array[i].pid, i);
		}
		// clear prog_array[] for next line
		k = 0;
		tmp[k++] = NULL;
		while(tmp[k] != NULL){
			free(tmp[k]); // because I used strdup
			k++;
		}
	}
	active_processes = num_procs;
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

void read_file(int fd){
	/*
	Reads fd into prog_array and returns number of lines
	*/
	char buf[BUFSIZ];
	char word[BUFSIZ];
	int index = 0;
	int i = 0;
	int size = 0;
	
	// Read lines from fd into buf
	while((size = p1getline(fd, buf, BUFSIZ)) != 0){
		num_procs += 1;
		if(buf[size - 1] == '\n'){
			buf[size - 1] = '\0';
		}
		// first word of each line is going to be the program
		index = p1getword(buf, index, word);
		prog_array[i].program = p1strdup(word);


		// find number of words in the string
		int word_count = count_words(buf);
		// malloc MyObject.args with the number of words
		prog_array[i].args = malloc(word_count * sizeof(prog_array[i].args));

		// loop through each word in the line
		int arg_count = 0;
		while ((index = p1getword(buf, index, word)) != -1){
			prog_array[i].args[arg_count] = p1strdup(word);
			arg_count += 1;
		}
		// insert null to the end of the args list
		prog_array[i].args[arg_count] = NULL;
		// increment index of prog_array
		i++;
		// set index of the word to 0 so we can process next line
		index = 0;
		//buf[BUFSIZ] = "";
	}
}

void process_command_line(char **argv){
	char *filename = "";

	// process cmd line entry
	if (argv[optind] == NULL){ //No file specified. Processing stdin
		read_file(0); // 0 is the fd for stdin
	}else { // file present
		filename = argv[optind];
		int fd = open(filename, O_RDONLY); // open file for read only
		if (fd != -1){ // File exits
			read_file(fd);
		}else{
			p1perror(2, "Error: filename does not exist\n");
			exit(1);
		}
		close(fd);
	}
}

int main(int argc, char **argv){
	/*
 	 * main function that catches flags and opens/closes file
	 */
	int opt;
	char *p;
	char *q_char = NULL;
	int quantum;
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

	// find USPS_... variable
	if (((p = getenv("USPS_QUANTUM_MSEC")) != NULL) && !(ifQ)){
		quantum = p1atoi(p);
	}else if (ifQ){  // OVERIDE environment variable
		quantum = p1atoi(q_char);
	}else{
		p1perror(2, "Error finding USPS_QUANTUM_MSEC variable\n");
		return EXIT_FAILURE;
	}

	quantum = MS_PER_TICK * ((quantum + 1) / MS_PER_TICK);
	ticks_in_quantum = quantum / MS_PER_TICK;

	// Open file and send off to be processed
	process_command_line(argv);

	set_signals();

	execute(prog_array);
	purge(prog_array, num_procs);

	set_timer(quantum);

	alrm_handler(SIGALRM);

	while(active_processes > 0)
		pause();
	
	q->destroy(q);
}
