#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

const char *argp_program_version = "Steamloader 0.1";
const char *argp_program_bug_address = "martindisch@gmail.com";

static char doc[] =
    "Steamloader -- a tool that downloads Steam workshop items";
    
static struct argp argp = {0, 0, 0, doc};

int main(int argc, char **argv) {
    argp_parse(&argp, argc, argv, 0, 0, 0);
    return 0;
}
