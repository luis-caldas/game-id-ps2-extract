#include <stdio.h>

#define PRINT_NON_PRINTABLE '.'


// checks if the byte is inside the ascii range of readable chars
int is_ascii_readable(unsigned char char_input) {
    int ascii_byte_mm[2] = {0x20, 0x7E};
    return char_input >= ascii_byte_mm[0] && char_input <= ascii_byte_mm[1];
}

// prints a hex and char table to stdout
void print_hex(unsigned char *table, int table_size, int horizontal_size) {

    for (int i = 0; i < table_size;) {

        int now_horizontal = 0;

        // memory leakage
        if ((i + horizontal_size) > table_size) now_horizontal = horizontal_size - ((i + horizontal_size) - table_size);
        else now_horizontal = horizontal_size;

        printf("%016X - %0X - ", i, now_horizontal);

        for (int j = 0; j < horizontal_size; ++j)
            if (j < now_horizontal) printf("%02X ", table[i+j]);
            else printf("   ");

        printf("\t");

        for (int j = 0; j < horizontal_size; ++j)
            if (j < now_horizontal) printf("%c", is_ascii_readable(table[i+j]) ? table[i+j] : PRINT_NON_PRINTABLE);
            else printf(" ");

        printf("\n");

        i += now_horizontal;
    }

}
