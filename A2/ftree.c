#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
// Add your system includes here.

#include "ftree.h"
#include "hash.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    
    struct TreeNode *root = NULL;

    root = malloc(sizeof(struct TreeNode));

    printf("root allocated at %p \n", root);    

    root->fname = malloc(sizeof(256*sizeof(char)));
    

    root->contents = malloc(sizeof(struct TreeNode*));

    root->next = malloc(sizeof(struct TreeNode*));


    //printf("file name allocated \n");

    //strcpy(root->fname, fname);
    root->fname = fname;

    printf("file name : %s at %p \n", root->fname, root);

    struct stat buf;
    lstat(fname, &buf);

    int is_dir = S_ISDIR(buf.st_mode);
    int is_reg = S_ISREG(buf.st_mode);
    int is_link = S_ISLNK(buf.st_mode);
    int permissions = buf.st_mode & 0777; 

    if( is_link || is_reg ){

        FILE *file_to_read = fopen( fname, "rb" );
	root->hash = hash(file_to_read);

	//printf("hash created successfully \n");

	root->contents = NULL;
	root->next = NULL;

	fclose(file_to_read);

    } else if ( is_dir ) {

        DIR *dirstream = opendir(fname);
    	//struct dirent *currfile = readdir(dirstream);
	struct dirent *currfile = NULL;
	struct TreeNode *currnode = NULL;
	struct TreeNode *lastnode = NULL;

	//printf("currfile gotten \n");
	
	//contents of root must be set manually

	currfile = readdir(dirstream);
	if(currfile != NULL){

	    //printf("first file in dir is: %s \n",currfile->d_name);
	    currnode = generate_ftree(currfile->d_name);
	    //printf("first dir recusive call complete");
	    root->contents = currnode;
	    lastnode = currnode;

	}

	while ( currfile != NULL ){
	
	    currfile = readdir(dirstream);

	    if(currfile == NULL || currfile->d_name[0] == '.'){
		//printf("node is invisble or null \n", currfile->d_name); 
		continue;	    
	    }

	    currnode = generate_ftree(currfile->d_name);
	    
	    //printf("recursive call successful \n");

	    if(currnode != NULL){
		//printf("node isn't null \n");
	        //lastnode = currnode;
		//currnode->next = lastnode;
		lastnode->next = currnode;
		lastnode = lastnode->next;
		
		//printf("move to next node successful \n");
	    } else {

		printf("current node is null \n");		

	    }
	    //currfile = readdir(dirstream);
	} 
	
	printf("directory if done \n");
    }
    
    //printf("returned node %s at address %p \n", root->fname, root );
    return root;
}

void print_spaces(int x) {
	for(int i = 0; i < x; i++){
	    printf(" ");
	}
}

//Recursive call to print tree
void print_ftree_rec(struct TreeNode *root, int depth) {
    //printf("beginning to print tree \n");
    //if file, print.
    if(root->contents == NULL){
	 print_spaces(depth * 2);
         printf("%s (perms) \n", root->fname);
    } else { //if directory, go throught contents and recursively call each
	print_spaces(depth * 2);
        printf("====  %s (%d) ==== \n",  root->fname, root->permissions);
	struct TreeNode *currnode;
	currnode = root->contents;
	while(currnode->next != NULL){
		print_ftree_rec(currnode, depth + 1);
		currnode = currnode->next;
	}
	
    }
}



/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.

    //printf("%*s", depth * 2, "");
    // Your implementation here.

    print_ftree_rec(root, 0); //recursive call to recursive function

    

}

