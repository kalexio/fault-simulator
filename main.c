#include <getopt.h>
#include "define.h"

const char* circuit_name;
const char* fault_name;
int nodummy;

FILE *circuit_fd, *fault_fd;

void option_set (int argc, char* const argv[]);
void handle_files (const char*, const char*);

/* Description of long options for get_opt long */
static const struct option long_options[] = {
    { "help",       0, NULL, 'h' },
    { "circuit",    1, NULL, 'c' },
    { "fault",      1, NULL, 'f' }
};

static const char* const short_options = "hc:f:";

static const char* const usage_template = 
    "Usage: %s [ options ]\n"
    "   -h, --help          Print this information\n"
    "   -c, --circuit FILE  Read the circuit file\n"
    "   -f, --fault FILE    Read the fault file\n";

static void print_usage (int is_error);

int main (int argc, char* const argv[])
{
	nodummy = 0;
    program_name = argv[0];
    option_set(argc,argv);
    handle_files (circuit_name, fault_name);

    if (read_circuit (circuit_fd, circuit_name) < 0)
        system_error ("read_circuit");
    fclose (circuit_fd);
    
    if (nog<=0 || nopi<=0 || nopo<=0) {
		fprintf(stderr,"Error in circuit file: #PI=%d, #PO=%d, #GATES=%d\n",nopi,nopo,nog);
		abort();
	}
    
    nodummy = add_PO();
    allocate_stacks();
    maxlevel = compute_level();
    
    
    
    return 0;
}





/* Gets the user input arguments */
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

void handle_files (const char* circuit_name, const char* fault_name)
{
	
	if (circuit_name != NULL) {
        printf("circuit file= %s\n",circuit_name);
        circuit_fd = fopen (circuit_name, "r");
		if (circuit_fd == NULL)
			system_error ("fopen");
	}
	else {
		fprintf (stderr, "You must give a circuit file first\n");
		exit(1);
	}
	
    if (fault_name != NULL)
		printf("fault file= %s\n", fault_name);
        fault_fd = fopen (fault_name, "r");
		if (circuit_fd == NULL)
			system_error ("fopen");
}

static void print_usage (int is_error)
{
    fprintf (is_error ? stderr : stdout, usage_template, program_name);
    exit (is_error ? 1 : 0);
}
