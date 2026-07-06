#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_RAND (LONG_MAX/255255255255)
#define DEBUG 0
#define RAND_SEED 20007

#define INITIAL_SIZE 10
#define INITIAL_NUM 1
#define MAX_SIZE 1000
#define MAX_NUM 20

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

    printf("num_tbls,size_tbls,memory,accuracy\n"); // CSV format

    // Test how changing number of tables and size of tables affects CMS accuracy
    for (num_tbls=INITIAL_NUM; num_tbls<=MAX_NUM; num_tbls+=1){
        for (size_tbls=INITIAL_SIZE; size_tbls<=MAX_SIZE; size_tbls+=50){
            count_min_sketch(data_input, num_add, threshold, num_tbls, size_tbls);
        }
    }

    free(data_input);
    return 0;
}

void count_min_sketch(long data_input[], int num_add, int threshold, int num_tbls, int size_tbls){
/*  Dynamically allocate num_tbls many Count Min Sketch tables of size size_tbls
    Call hash() on each num_add many of data_input[]
    Storing each IP in the num_tbls many Count Min Sketch tables of size size_tbls
    Calculates the accuracy of CMS 
*/
    int** tables = (int**)malloc(num_tbls*sizeof(int*));
    assert(tables != NULL);
    int* tables_indices = (int*)malloc(num_tbls*sizeof(int));
    assert(tables_indices != NULL);
    char *flags = (char *)malloc(num_add * sizeof(char));
    assert(flags != NULL);
    memset(flags, 0, num_add * sizeof(char));

    // Dynmically allocate CMS tables
    for (int i=0; i<num_tbls; i++){
        tables[i] = (int*)malloc(size_tbls*sizeof(int));
        assert(tables[i] != NULL);
    }

    // Initialise frequency of IPs in each table to be 0
    for (int i=0; i<num_tbls; i++){
        for (int k=0; k<size_tbls; k++){
            tables[i][k] = 0;
        }
    }

    // Insert all IPs into CMS
    for (int j=0; j<num_add; j++){
        hash(data_input[j], tables_indices, num_tbls, size_tbls);

        int min=INT_MAX;

        // Find estimated frequency of each IP address
        // by taking the minimum across all hash tables
        for (int i=0; i<num_tbls; i++){
            tables[i][tables_indices[i]]++;
            if (tables[i][tables_indices[i]] < min){
                min = tables[i][tables_indices[i]];
            }
        }

        // Check if IP address frequency is above threshold
        if (min >= threshold){
            flags[j] = 1;
        }
    }

    int predicted_flag_count=0;
    int actual_flag_count=0;

    // Check each unique IP to see if CMS flags it 
    // and if its frequency is actually above the threshold
    for (int j=0; j<num_add; j++){

        // IP address is not flagged by CMS
        if (flags[j] != 1){
            continue;
        }

        // Check if IP address is counted previously or not
        int already_seen = 0;

        // Check previously checked IP addresses to see if they are the same
        // and if they are both flagged - it is possible that it was
        // not flagged the previous time but is flagged this time
        for (int k=0; k<j; k++){
            if (data_input[k] == data_input[j] && flags[k] == flags[j]){
                already_seen=1; 
                break; 
            }
        }

        if (already_seen){
            continue;
        }

        // Check if the IP actually has frequency above threshold
        int actual_freq = calc_freq(data_input, num_add, data_input[j]); 
        if (actual_freq >= threshold) {
            actual_flag_count++;
        }
        predicted_flag_count++;
    }

    // Calculate the accuracy using precision formula 
    // TP / (TP + FP)
    double accuracy; 
    if (predicted_flag_count == 0){
        accuracy = 1.0;  // No false positives if nothing was flagged
    } else {
        accuracy = (double)actual_flag_count / predicted_flag_count;
    }
    
    int memory=num_tbls*size_tbls*sizeof(int);

    printf("%d,%d,%d,%f\n", num_tbls, size_tbls, memory, accuracy);

    // Free each hash table
    for (int i=0; i<num_tbls; i++){
        free(tables[i]);
        tables[i] = NULL;
    }
    
    free(tables);
    tables = NULL;

    free(tables_indices);
    tables_indices = NULL;

    free(flags);
    flags = NULL;

    //Dummy call to hash to free dynamic memory returned from hash function
    //free(hash(0,NULL,0,0));
}

/* Hashs a long to n many hash table indices stored in A for use in an array of dimension size
   Returns a pointer to dynamically allocated list of random integers used for hashing */
static int* hash(long ip, int A[], int n, int size){
    static int* rands = NULL;
    static int allocated = 0;

    if (n > allocated) {
        rands = realloc(rands, n * sizeof(int));
        assert(rands != NULL);

        for (int i = allocated; i < n; i++) {
            rands[i] = 0;
        }

        allocated = n;
    }
    for( int i = 0 ; i < n ; i++){
        if(rands[i]==0){rands[i]=rand();}
        A[i] = (ip*(rands[i]%MAX_RAND))%size;
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


