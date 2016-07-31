#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

#include "steamaccess/steamaccess.h"

// version & address for version & help
const char *argp_program_version = "Steamloader 0.1";
const char *argp_program_bug_address = "martindisch@gmail.com";

// documentation
static char doc[] =
    "Download Steam workshop item(s) using STEAM_ID(s).";

// accepted non-option arguments
static char args_doc[] = "STEAM_ID...";

// the options
static struct argp_option options[] = {
    {"output", 'o', "DIR", 0, "Set download path"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"silent", 's', 0, OPTION_ALIAS}
};

// structure to store arguments
struct arguments {
    char **args;
    int silent;
    char *output;
    int id_count;
    int args_size;
};

// parser for a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    // get reference to arguments structure
    struct arguments *arguments = state->input;
    
    switch (key) {
        case 'o':
            arguments->output = arg;
        break;
        case 'q': case 's':
            arguments->silent = 1;
        break;
        case ARGP_KEY_ARG:
            arguments->id_count++;
            // decide if we need to allocate more memory for arguments
            if (arguments->id_count > arguments->args_size) {
                int new_size = arguments->args_size * 2;
                char **temp = realloc(arguments->args, new_size * sizeof(char *));
                if (temp) {
                    arguments->args = temp;
                    arguments->args_size = new_size;
                } else {
                    printf("Ran out of memory processing arguments\n");
                    exit(1);
                }
            }
            arguments->args[state->arg_num] = arg;
        break;
        case ARGP_KEY_END:
            if (state->arg_num < 1) argp_usage(state);
        break;
        
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// the parser object
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
    struct arguments arguments;
    
    // set default values
    arguments.silent = 0;
    arguments.output = "-";
    arguments.id_count = 0;
    arguments.args = malloc(20 * sizeof(char *));
    arguments.args_size = 20;
    
    // parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    
    /**
     * Debug code
     */
    struct fileinfo *result = get_fileinfo("722648525");
    if (result) {
        printf("%s\n%s\n", result->filename, result->download);
        // flush to make sure printf is done before we start freeing memory
        fflush(stdout);
        free(result->filename);
        free(result->download);
        free(result);
    } else {
        printf("Unable to obtain result\n");
    }
    
    free(arguments.args);
    return 0;
}
