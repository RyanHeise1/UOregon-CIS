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

    while (fgets(buf, sizeof buf, stdin) != NULL) {         
    	bool matches;         
    	matches = (strcmp(last, buf) == 0);  /* see if the lines match */         

    	if (! matches) {                     /* NO */             
    		if (! first)                     /* not first line, print last */                 
    			printf("%s"  , last);             
    		else                             /* was first line */                 
    			first = false;             
    		strcpy(last, buf);               /* save line for next compare */        
    	}    
    }    
    if (! first)                             /* write out the last line */         
    	printf("%s"  , last);

    return exitStatus; 
}