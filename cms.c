/*
Count-Min Sketch implementation for streaming IP address frequency estimation.
Implements a probabilistic streaming algorithm for detecting high-frequency IP
addresses using multiple hash tables.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_RAND (LONG_MAX/255255255255)
#define DEBUG 0
#define RAND_SEED 20007

long read_ip();
void print_ip(long num);
int calc_freq(long A[], int n, long k);
static int* hash(long ip, int A[], int n, int size);

void count_min_sketch(long data_input[], int num_add, int threshold, int num_tbls, int size_tbls);

int main(int argc, char **argv){
    
    srand(RAND_SEED);

    int num_add;
    int threshold;
    int num_tbls;
    int size_tbls;
    scanf("Number of addresses = %d\n", &num_add);
    scanf("Threshold of accesses = %d\n", &threshold);
    scanf("Number of tables = %d\n", &num_tbls);
    scanf("Size of tables = %d\n",&size_tbls);

    long *data_input = (long *) malloc(sizeof(long)*num_add);
    assert(data_input);

    if(DEBUG) printf("Read in %d addresses:\n",num_add);
    for(int i = 0 ; i < num_add ; i++){
        data_input[i]=read_ip();
        if(DEBUG) {
            print_ip(data_input[i]);
            printf("\n");
        }
    }

    count_min_sketch(data_input, num_add, threshold, num_tbls, size_tbls);

    free(data_input);
    return 0;
}

void count_min_sketch(long data_input[], int num_add, int threshold, int num_tbls, int size_tbls){
/*  Dynamically allocate num_tbls many Count Min Sketch tables of size size_tbls
    Call hash() on each num_add many of data_input[]
        Storing each IP in the num_tbls many Count Min Sketch tables of size size_tbls
        Flagging any ip addresses pinged equal or more than the threshold
    If no ip addresses are flagged, print the confirmation message
*/
    int** tables = (int**)malloc(num_tbls*sizeof(int*));
    assert(tables != NULL);
    int* tables_indices = (int*)malloc(num_tbls*sizeof(int));
    assert(tables_indices != NULL);

    // Dynmically allocate CMS tables
    for (int i=0; i<num_tbls; i++){
        tables[i] = (int*)malloc(size_tbls*sizeof(int));
        assert(tables[i] != NULL);
    }

    // Initialise each cell in each table to be 0
    for (int i=0; i<num_tbls; i++){
        for (int k=0; k<size_tbls; k++){
            tables[i][k] = 0;
        }
    }
    
    // Flag to check if any IP addresses are above threshold
    int above_threshold=0;

    // Update each Count-Min Sketch table using independent hash functions.
    // The estimated frequency of an IP is taken as the minimum counter across
    // all hash tables.
    for (int j=0; j<num_add; j++){
        hash(data_input[j], tables_indices, num_tbls, size_tbls);

        int min=INT_MAX;

        // Find estimated frequency of the IP address
        // by taking the minimum across all hash tables
        for (int i=0; i<num_tbls; i++){
            tables[i][tables_indices[i]]++;
            if (tables[i][tables_indices[i]] < min){
                min = tables[i][tables_indices[i]];
            }
        }

        // Print message if above threshold
        if (min >= threshold){
            printf("Flagging ip address: ");
            print_ip(data_input[j]);
            printf(" up to %d accesses.\n", min);
            above_threshold = 1;
        }
    }

    if (above_threshold == 0){
        printf("No IP addresses above threshold.\n");
    }

    // Free each hash table
    for (int i=0; i<num_tbls; i++){
        free(tables[i]);
        tables[i] = NULL;
    }
    
    free(tables);
    tables = NULL;

    free(tables_indices);
    tables_indices = NULL;

    //Dummy call to hash to free dynamic memory returned from hash function
    free(hash(0,0,0,0));
}


/* Hashs a long to n many hash table indices stored in A for use in an array of dimension size
   Returns a pointer to dynamically allocated list of random integers used for hashing */
static int* hash(long ip, int A[], int n, int size){
    static int* rands = NULL;
    if(!rands) rands = calloc(sizeof(int),n);
    for( int i = 0 ; i < n ; i++){
        if(rands[i]==0){rands[i]=rand();}
        A[i] = (ip*(rands[i]%MAX_RAND))%size;
        //printf("A[%d]=%d, rands = %ld\n",i,A[i], rands[i]%MAX_RAND);
    }
    return rands;
}

/* Can be used for experimentation to calculate the actual frequency a value k occurs in array A of size n*/
int calc_freq(long A[], int n, long k){
    int count = 0;
    for(int i = 0 ; i < n ; i++){
        if(A[i]==k)count++;
    }
    return count;
}

/* Reads an IP address from stdin and returns its value represented as a long*/
long read_ip(){
    int a,b,c,d;
    scanf("%d.%d.%d.%d\n",&a,&b,&c,&d);
    //Note, conversions for each three digit representations to a single value long
    return (long)d+(long)c*1000+(long)b*1000000+(long)a*1000000000;
}

/* Prints a long in the format of an IP address, i.e., four period seperated three digit values*/
void print_ip(long num){
    //Note, conversions for the long into each three digit representations
    printf("%03d.%03d.%03d.%03d",(int)(num/1000000000), (int)(num%1000000000/1000000), (int)(num%1000000/1000), (int)(num%1000));
}



