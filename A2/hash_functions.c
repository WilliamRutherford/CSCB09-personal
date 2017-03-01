#include <stdio.h>
#include <stdlib.h>

char *hash(FILE *in_file) {

    //returns an 8byte hash
    //a hex digit stores 16 bits, 2 bytes
    //therefore, 8 bytes is 4 hex digits
    // this means our block size is 4.

    char curr_byte;
    int i = 0;
    char *hash_val = malloc(8*sizeof(char));

    while (fscanf(in_file, "%c", &curr_byte) != EOF ){
 
        hash_val[i % 8] = hash_val[i % 8] ^ curr_byte;

        i++;
    }

    //void *mem_pos = malloc(sizeof(hash_val));
    

    return hash_val;

}
/*
void hash(char *hash_val, long block_size) {
	
    char curr_byte;	
    int i = 0;

    while( scanf("%c",&curr_byte) != EOF ){

	hash_val[i % block_size] = hash_val[i % block_size] ^ curr_byte;

	i++;
    }
}
*/

int check_hash(const char *hash1, const char *hash2, long block_size) {
    int i = 0, result = -1;

    while(result == -1){
	if( hash1[i] != hash2[i] ){
	    
	    result = i; //we found the first difference
	
	} else if ( i == block_size ) {
		result = block_size; //there are no differences
	}


	i++;
    }
    
    return result;
}
