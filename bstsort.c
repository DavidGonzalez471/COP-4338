//David Gonzalez
//6073443
//I affirm this program is all of my original work.
//This program can compare an input of strings against each //other including removing those that aren't capitalized or //lowercase
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//string comparator
int str_cmp(char *s1, char *s2)
{
    while(*s1 && *s2 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    
    return (*s1 - *s2);
}



struct node
{
    char value[100];
    int count;
    struct node *rightChild;
    struct node *leftChild;
};

//binary search tree structure
struct node* insert(char key[100], struct node **leaf)
{
    if(*leaf == NULL)
    {
        *leaf = (struct node*) malloc (sizeof (struct node));
        strcpy((*leaf) -> value, key);
        (*leaf) -> count = 1;
        (*leaf) -> leftChild = NULL;
        (*leaf) -> rightChild = NULL;
    }
	else if(str_cmp(key, (*leaf) -> value) == 0)
	{
		(*leaf) -> count++;
	}
	else if(str_cmp(key, (*leaf) -> value) > 0)
	{
		insert(key, &(*leaf) -> rightChild);
	}
	else if(str_cmp(key, (*leaf) -> value) < 0)
	{
		insert(key, &(*leaf) -> leftChild);
	}
}

//inorder traversal method
void inorder_print(struct node *leaf, FILE * out)
{

    
    if(leaf != NULL)
    {

        inorder_print((leaf -> leftChild), out);

	  fprintf(out, "%s\n", (leaf -> value));

            

        inorder_print((leaf -> rightChild), out);
    }
}

//method to filter cflag output using isupper
void inorder_print_c(struct node *leaf, FILE * out)
{

    
    if(leaf != NULL)
    {
	int capitalCounter = 0;
        inorder_print_c((leaf -> leftChild), out);
       
            for(int i = 0; i < sizeof(leaf -> value); i++)
            {

                if(isupper(leaf-> value[i]) != 0)
                {
                    capitalCounter++;
                }

            }
            if(capitalCounter == strlen(leaf -> value) -1)
            {
                    fprintf(out, "%s\n", (leaf -> value));
            }

        inorder_print_c((leaf -> rightChild), out);
    }
}

//method to filter lflag output using islower
void inorder_print_l(struct node *leaf, FILE * out)
{

    
    if(leaf != NULL)
    {
	int capitalCounter = 0;
        inorder_print_l((leaf -> leftChild), out);
       
            for(int i = 0; i < sizeof(leaf -> value); i++)
            {

                if(islower(leaf-> value[i]) != 0)
                {
                    capitalCounter++;
                }

            }
            if(capitalCounter == strlen(leaf -> value) -1)
            {
                    fprintf(out, "%s\n", (leaf -> value));
            }

        inorder_print_l((leaf -> rightChild), out);
    }
}


void delete_tree(struct node *leaf)
{
    if(leaf != NULL)
    {
        delete_tree(leaf -> leftChild);
        delete_tree(leaf -> rightChild);
        free(leaf -> value);

    }
}

//main method uses getopt to capture user input for cflag or lflag and decide how to organize bst output
int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, l, err = 0;
	int cflag, lflag, oflag = 0;	


	static char usage[] = "Usage: bstsort -c -l [-o output_file_name] [input_file_name]";

	FILE * input = stdin;		
	FILE * output = stdout;		

	char contents[100];		
	char * sentence;		
	struct node * root = NULL;						


	while ((c = getopt(argc, argv, "clo:")) != -1)
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'l':
		    lflag = 1;
		    break;
		case 'o':
			oflag = 1;
			output = fopen(optarg, "w");
			break;
		case '?':
			printf("%s", usage);
			break;
		}





	if(argv[optind] != NULL)
	{

		input = fopen(argv[optind], "r");


		if(input == NULL)
		{
			printf("File not found.\n");	// Print file not found
			exit(0);	
		}
	}

	    while(fgets(contents, sizeof(contents), input) != NULL)
	    { 

		    sentence = (char *)malloc(strlen(contents));
		

		    strcpy(sentence, contents);
		    

		    insert(sentence, &root); 
	    }

	if(cflag == 1)
	{
	    inorder_print_c(root, output);
	}
	else if(lflag == 1)
	{
	    inorder_print_l(root, output);
	}
	else
	{
	    inorder_print(root, output);
	}


	delete_tree(root);
}
