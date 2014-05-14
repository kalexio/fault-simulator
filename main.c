#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include "define.h"

const char* circuit_name;
const char* fault_name;
const char* vectors_name;
int nodummy;
char test_name[100]="";

int* LUT;
FILE *circuit_fd, *fault_fd, *vectors_fd,*test_fd;

/* Description of long options for get_opt long */
static const struct option long_options[] = {
    { "help",       0, NULL, 'h' },
    { "circuit",    1, NULL, 'c' },
    { "vector",     1, NULL, 'v' },
    { "fault",      1, NULL, 'f' }
};

static const char* const short_options = "hc:v:f:";

static const char* const usage_template = 
    "Usage: %s [ options ]\n"
    "   -h, --help          Print this information\n"
    "   -c, --circuit FILE  Read the circuit file\n"
    "   -v, --vectors FILE  Read the test-pattern file\n"
    "   -f, --fault FILE    Read the fault file\n";

static void print_usage (int is_error);




int main (int argc, char* const argv[])
{
	int i,j;
	char c;
	nodummy = 0;
    program_name = argv[0];
	double u1, u2, total = 0;
	struct timeval tv;
    
    /* Get the input arguments */
    option_set(argc,argv);
    
    /* Handle the input files */
    handle_files (circuit_name,vectors_name);
    
    
    gettimeofday(&tv,NULL);
   	u1 = tv.tv_sec*1.0e6 + tv.tv_usec;


	/* Read the circuit file and make the structures */
    if (read_circuit (circuit_fd) < 0)
        system_error ("read_circuit");
    fclose (circuit_fd);
    
    if (nog<=0 || nopi<=0 || nopo<=0) {
		fprintf(stderr,"Error in circuit file: #PI=%d, #PO=%d, #GATES=%d\n",nopi,nopo,nog);
		abort();
	}
    
    /* Add a gate for the output stage as you did for the input stage */
    nodummy = add_PO();
    
    /* Compute the levels of the circuit */
    allocate_stacks();
    maxlevel = compute_level();
	place_PO();

	
    printf("the max level = %d\n",maxlevel);
    
    /* Computes the level of each gate */
    allocate_event_list();
    levelize();
    //xfree(event_list); 
    
    gettimeofday(&tv,NULL);
    u2 = tv.tv_sec*1.0e6 + tv.tv_usec;
    
    total=(u2-u1);
    
    printf("Time for construction of data structures: %f usec\n", total);
    total= 0;
    
	printf("opening vectors file= %s\n",vectors_name);
    vectors_fd = fopen (vectors_name, "r");
	if (vectors_fd == NULL)
		system_error ("fopen");
		
	gettimeofday(&tv,NULL);
   	u1 = tv.tv_sec*1.0e6 + tv.tv_usec;
		
	/* Read the vector file and put the input values to the INPUT GATES */
	if (read_vectors (vectors_fd,vectors_name) != 0)
		system_error ("read_vectors");
	//fclose (vectors_fd);  //valgrind mistake
	
	
	
	//logic simulation here
	LUT = create_lut (LUT);
	logic_sim();

	i=0; j=0;
	if(test_name[0]=='\0') {
		while((c=circuit_name[i++])!='\0') {
			if(c=='/') j=0;
			else if(c=='.') break;
			else test_name[j++]=c;
		}
		test_name[j]='\0';
		strcat(test_name,".test");
	}

	gettimeofday(&tv,NULL);
    u2 = tv.tv_sec*1.0e6 + tv.tv_usec;
    
    total=(u2-u1);
    
    printf("Time for logic simulation: %f usec\n", total);
    total= 0;

    //print_logic_sim();
	

	//<----------------------------------------------------------------
	//fault simulation here
	gettimeofday(&tv,NULL);
   	u1 = tv.tv_sec*1.0e6 + tv.tv_usec;	
	
	create_fault_list ();
	//print_fault_list();
	fault_sim();
	
	gettimeofday(&tv,NULL);
    u2 = tv.tv_sec*1.0e6 + tv.tv_usec;
    
    total=(u2-u1);
    
    printf("Time for fault simulation: %f usec\n", total);
    total= 0;
	
	
	
	/*if ( fault_name == NULL ) {
		printf("\nWe are done\n");
		return 0;
	}
	
	printf("opening fault file= %s\n",fault_name);
    vectors_fd = fopen (vectors_name, "r");
	if (vectors_fd == NULL)
		system_error ("fopen"); */
	
	
		
	//	synexeia simulation<-----------------------------------
    
    
    
    printf("\nWe are done\n");
    return 0;
}




void option_set (int argc, char* const argv[])
{	
	int next_option;

    do {
        next_option = 
            getopt_long (argc, argv, short_options, long_options, NULL);
        switch (next_option) {
            case 'c':
                circuit_name = optarg;
				break;
			case 'v':
				vectors_name = optarg;
				break;
            case 'f':
                fault_name = optarg;               
				break;
            case 'h':
                print_usage (0);
            case '?':
                print_usage (1);
            case -1:
				if (argc > 1)
					break;
                else
					print_usage (0);
            default:
                abort ();
        }
    } while (next_option != -1);

    if (optind != argc)
        print_usage (1);
}




void handle_files (const char* circuit_name,const char* vectors_name)
{
	if ((circuit_name != NULL) & (vectors_name != NULL) ) {
        printf("opening circuit file= %s\n",circuit_name);
        circuit_fd = fopen (circuit_name, "r");
		if (circuit_fd == NULL)
			system_error ("fopen");
	}
	else {
		fprintf (stderr, "You must give a circuit file and a test pattern file first\n");
		exit(1);
	}
			
}




static void print_usage (int is_error)
{
    fprintf (is_error ? stderr : stdout, usage_template, program_name);
    exit (is_error ? 1 : 0);
}

