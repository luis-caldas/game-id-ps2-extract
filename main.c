// standard libs
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// confi headers
#include "config.h"

// own objects
#include "memrmem.h"


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

// checks if the byte is inside the ascii range of readable chars
int is_ascii_readable(unsigned char char_input) {
    int ascii_byte_mm[2] = {0x20, 0x7E};
    return char_input >= ascii_byte_mm[0] && char_input <= ascii_byte_mm[1];
}

// prints a hex and char table to stdout
void print_hex(unsigned char *table, int table_size) {

    for (int i = 0; i < table_size;) {

        int now_horizontal = 0;

        // memory leakage
        if ((i + HORIZONTAL_SIZE) > table_size) now_horizontal = HORIZONTAL_SIZE - ((i + HORIZONTAL_SIZE) - table_size);
        else now_horizontal = HORIZONTAL_SIZE;

        printf("%016X - %0X - ", i, now_horizontal);

        for (int j = 0; j < HORIZONTAL_SIZE; ++j)
            if (j < now_horizontal) printf("%02X ", table[i+j]);
            else printf("   ");

        printf("\t");

        for (int j = 0; j < HORIZONTAL_SIZE; ++j)
            if (j < now_horizontal) printf("%c", is_ascii_readable(table[i+j]) ? table[i+j] : PRINT_NON_PRINTABLE);
            else printf(" ");

        printf("\n");

        i += now_horizontal;
    }
}

// extract a subblock of memory from another piece of memory
void extract_block(int offset, int block_size, unsigned char *table, unsigned char* block) {
    memcpy(block, &table[offset], block_size * sizeof(*block));
}

unsigned char *find_bytes(unsigned char *byte_array_to_search, int byte_array_size,
               unsigned char *table, int table_size, int start_offset) {

    unsigned char *pointer = memrmem(table, table_size, byte_array_to_search, byte_array_size);

    return pointer;

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
    print_hex(usable_block, BLOCK_SIZE);

    printf("\n\n");

    print_hex(find_bytes(
        PRECEDING_DATA, PRECEDING_DATA_SIZE,
        usable_block, BLOCK_SIZE,
        0
    ), 0x20);

    return 0;
}
