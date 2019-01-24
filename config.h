// memory configs
#define USABLE_HEADER 0x927C0
#define BLOCK_SIZE 0x800
#define CODE_BLOCK_OFFSET 0x82800

#define PRECEDING_DATA_SIZE 9
#define SUCCEEDING_DATA_SIZE 4
unsigned char PRECEDING_DATA[] = {0x24, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x0D};
unsigned char SUCCEEDING_DATA[] = {0x3B, 0x31, 0x00, 0x00};

// verbose configs
#define HORIZONTAL_SIZE 0x20
#define PRINT_NON_PRINTABLE '.'
