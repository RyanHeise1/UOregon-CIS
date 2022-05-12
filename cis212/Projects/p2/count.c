#include <stdio.h>   /* defines BUFSIZ, fgets(), and printf() */ 
#include <stdlib.h>  /* defines EXIT_SUCCESS */ 
#include <stdbool.h> /* defines bool, false, and true */ 
#include <string.h>  /* defines strcmp() and strcpy() */ 
#define UNUSED __attribute__((unused)) 

int main(UNUSED int argc, UNUSED char *argv[]) {
    char buf[BUFSIZ];
    char last[BUFSIZ] = "";   
    bool first = true;     
    int exitStatus = EXIT_SUCCESS;     
    int matchCt = 1;

    while (fgets(buf, sizeof buf, stdin) != NULL) {         
    	bool matches;
    	matches = (strcmp(last, buf) == 0);    /* see if the lines match... 1 if they match 0 if they don't*/         

    	if (! matches) {                                       /* NO match if(matches) to test if they match*/
            if (! first)                                              /* not first line, print last */
                printf("%7d %s", matchCt, last);
    		else                                                      /* was first line */                 
    			first = false;
            strcpy(last, buf);                                 /* save line for next compare */
            matchCt = 1;                                       /* reset counter */
    	} else{                                                      /*if match increase counter for that word*/
            matchCt += 1;
        }   
    }
    if (! first)                                                        /* write out the last line */         
    	printf("%7d %s", matchCt, last);

    return exitStatus; 
}