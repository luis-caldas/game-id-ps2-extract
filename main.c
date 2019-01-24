#include <stdlib.h>
#include <argp.h>

// program version
const char *argp_program_version = "0.1";

// documentation
static char doc[] = "\nSimple PS2 ISO game ID extractor";
static char args_doc[] = "file_name";

struct arguments {
    char *args[1];
};

// parsing
static error_t
parse_opt (int key, char *arg, struct argp_state *state) {

    /* Get the input argument from argp_parse, which we
    know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {

        case ARGP_KEY_ARG:

            if (state->arg_num >= 1)
                // too many arguments
                argp_usage (state);

            arguments->args[state->arg_num] = arg;
            break;

        case ARGP_KEY_END:

            if (state->arg_num < 1)
                // not enough arguments
                argp_usage (state);

            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

// the parser
static struct argp argp = { 0, parse_opt, args_doc, doc };


int main(int argc, char const *argv[]) {

    struct arguments arguments;

    // defaults
    arguments.args[0] = "";

    // parse our arguments
    // every option seen by parse_opt will be reflected in arguments
    argp_parse (&argp, argc, (char **)argv, 0, 0, &arguments);

    printf("file_name = %s\n", arguments.args[0]);

    return 0;
}
