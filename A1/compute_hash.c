#include <stdio.h>
#include <stdlib.h>


// Hash manipulation functions in hash_functions.c
void hash(char *hash_val, long block_size);
int check_hash(const char *hash1, const char *hash2, long block_size);

#ifndef MAX_BLOCK_SIZE
    #define MAX_BLOCK_SIZE 1024
#endif

/* Converts hexstr, a string of hexadecimal digits, into hash_val, an an array of char.
 * Each pair of digits in hexstr is converted to its numeric 8-bit value and stored in
 * an element of hash_val.
 * Preconditions: 
 *    - hash_val must have enough space to store block_size elements
 *    - hexstr must be block_size * 2 characters in length
 */

void xstr_to_hash(char *hash_val, char *hexstr, int block_size) {
    for(int i = 0; i < block_size*2; i += 2) {
        char str[3];
        str[0] = hexstr[i];
        str[1] = hexstr[i + 1];
        str[2] = '\0';
        hash_val[i/2] = strtol(str, NULL, 16);
    }
}

/* Print the values of the char array hash_val in hex
 * Note that each char (8 bits) corresponds to two digits in hex (4 bits per digit)
 * In case you are wondering about the %.2hhx in printf:
 * -- The 2 makes sure that for each char always two digits are printed in hex notation, so that for example
 * for a '\0' (all bits in the byte are zero) two zeros are printed in hex notation, rather than one.
 * -- The hh makes sure that printf treats the char properly, since %x by default expects an int.
 */

void show_hash(char *hash_val, long block_size) {
    for(int i = 0; i < block_size; i++) {
        printf("%.2hhx ", hash_val[i]);
    }
    printf("\n");
}


int main(int argc, char **argv) {
    char hash_val[MAX_BLOCK_SIZE] = {'\0'};
    int block_size = strtol(argv[1], NULL, 10);
    if(block_size > MAX_BLOCK_SIZE || block_size <= 0) {
	
	printf("The block size should be a positive integer less than or equal to %d.\n", MAX_BLOCK_SIZE);
	
    } else  if(argc == 2){

	hash(hash_val, block_size);
	
	printf("\n");

	show_hash(hash_val, block_size);

    } else {

	hash(hash_val, block_size);
	printf("\n");
	char comparison_hash[MAX_BLOCK_SIZE] = {'\0'}; 
	xstr_to_hash(comparison_hash, argv[2], block_size);
	printf("%d", check_hash(hash_val, comparison_hash, block_size));

    }

    printf("SOME TEST CASES:\n");
    char tc1a[1] = {'1'};
    char tc1b[1] = {'2'};
    printf("should be 0: %d \n", check_hash(tc1a, tc1b, 1));
    char tc2a[3] = {'1', '2', '3'};
    char tc2b[3] = {'1', '3', '3'};
    printf("should be 1: %d \n", check_hash(tc2a, tc2b, 3));
    char tc3a[5] = {'1', '2', '5', '5', '5'};
    char tc3b[3] = {'1', '2', '5'};
    printf("different sized arrays: %d \n", check_hash(tc3a, tc3b, 5));
    return 0;
}

