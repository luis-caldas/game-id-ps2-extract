// standard libs
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// confi headers
#include "config.h"

// own objects
#include "print_hex.h"
#include "memmem.h"


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

int extract_header_from_file (char *file_name_path, int header_size, unsigned char *header) {

    FILE *file_pointer;
    file_pointer = fopen(file_name_path, "r");

    if (file_pointer == NULL) return 0;

    fread(header, sizeof(char), header_size, file_pointer);

    if (ferror(file_pointer)) return 0;

    fclose(file_pointer);

    return 1;
}

// extract a subblock of memory from another piece of memory
void extract_block(int offset, int block_size, unsigned char *table, unsigned char* block) {
    memcpy(block, &table[offset], block_size * sizeof(*block));
}

int find_occurences(unsigned char *byte_array_to_search, int byte_array_size,
                    unsigned char *table, int table_size) {

    // first pass to count the occurences
    int occurences = 0;
    int variable_table_size = table_size;

    while(1) {

        unsigned char *pointer = memrmem(table, variable_table_size, byte_array_to_search, byte_array_size);

        if (pointer == NULL) break;
        else {
            ++occurences;
            variable_table_size = ((unsigned long long)*&pointer - (unsigned long long)*&table);
        }

    }

    return occurences;

}

void extract_occurences(unsigned char *byte_array_to_search, int byte_array_size,
                        unsigned char *table, int table_size, unsigned char *items_array_pointer[]) {

    // first pass to count the occurences
    int occurences = 0;
    int variable_table_size = table_size;

    while(1) {

        unsigned char *pointer = memrmem(table, variable_table_size, byte_array_to_search, byte_array_size);

        if (pointer == NULL) break;
        else {
            items_array_pointer[occurences] = pointer + byte_array_size; ++occurences;
            variable_table_size = ((unsigned long long)*&pointer - (unsigned long long)*&table);
        }

    }

}

void size_without_suffix(int total_items, int *size_array, unsigned char *pointers[],
                         unsigned char *suffix, int suffix_size, int pointer_maximum_size) {

    for (int i = 0; i < total_items; ++i) {
        unsigned char *pointer = memmem(pointers[i], pointer_maximum_size, suffix, suffix_size);
        size_array[i] = (int)((unsigned long long)*&pointer - (unsigned long long)*&pointers[i]);
    }

}

// clears a memory range
void clear_table(int table_size, unsigned char *table_pointer) {
    for (int i = 0; i < table_size; ++i)
        table_pointer[i] = 0;
}

int main(int argc, char const *argv[]) {

    struct arguments arguments;

    // defaults
    arguments.args[0] = "";

    // parse our arguments
    // every option seen by parse_opt will be reflected in arguments
    argp_parse (&argp, argc, (char **)argv, 0, 0, &arguments);

    // create the buffer for the file header
    unsigned char header[USABLE_HEADER];
    clear_table(USABLE_HEADER, header);

    // get the header from the file name
    if (!extract_header_from_file(arguments.args[0], USABLE_HEADER, header)) {
        printf("Error opening/reading file\n");
        return ENOENT;
    }

    // usable block that contains the game id
    unsigned char usable_block[BLOCK_SIZE];
    clear_table(BLOCK_SIZE, usable_block);

    // extract from the header to the created block
    extract_block(CODE_BLOCK_OFFSET, BLOCK_SIZE, header, usable_block);

    // a little of verbose
    print_hex(usable_block, BLOCK_SIZE, HORIZONTAL_SIZE);

    // find the number of items in the block
    int number_of_items = find_occurences(
        PRECEDING_DATA, PRECEDING_DATA_SIZE,
        usable_block, BLOCK_SIZE
    );

    // create array of pointers to get the offsets
    unsigned char *items_found_pointers[number_of_items];

    // extract the found occurences
    extract_occurences(
        PRECEDING_DATA, PRECEDING_DATA_SIZE,
        usable_block, BLOCK_SIZE,
        items_found_pointers
    );

    // array of the sizes of the items
    int items_found_sizes[number_of_items];
    for (int i = 0; i < number_of_items; ++i) items_found_sizes[i] = 0;

    size_without_suffix(number_of_items, items_found_sizes, items_found_pointers,
                        SUCCEEDING_DATA, SUCCEEDING_DATA_SIZE, MAXIMUM_BUFFER_CHECK);

    return 0;
}
