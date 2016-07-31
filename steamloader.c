#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "steamaccess/steamaccess.h"

// Version & address for version & help
const char *argp_program_version = "Steamloader 0.1";
const char *argp_program_bug_address = "martindisch@gmail.com";

// Documentation
static char doc[] =
    "Download Steam workshop item(s) using STEAM_ID(s).";

// Accepted non-option arguments
static char args_doc[] = "STEAM_ID...";

// The options
static struct argp_option options[] = {
    {"output", 'o', "DIR", 0, "Set download path"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"silent", 's', 0, OPTION_ALIAS}
};

// Structure to store arguments
struct arguments {
    char **args;
    int silent;
    char *output;
    int id_count;
    int args_size;
};

// Parser for a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    // Get reference to arguments structure
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
            // Decide if we need to allocate more memory for arguments
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

// The parser object
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
    struct arguments arguments;
    
    // Set default values
    arguments.silent = 0;
    arguments.output = "-";
    arguments.id_count = 0;
    arguments.args = malloc(20 * sizeof(char *));
    arguments.args_size = 20;
    
    // Parse arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    
    /**
     * Debug code
     */
    char *url = "https://api.steampowered.com/ISteamRemoteStorage/GetPublishedFileDetails/v0001/";
    char *fields = "itemcount=1&publishedfileids[0]=722648525";
    char *result = get_post(url, fields);
    if (result) {
        printf("%s\n", result);
    }
    
    free(arguments.args);
    return 0;
}
