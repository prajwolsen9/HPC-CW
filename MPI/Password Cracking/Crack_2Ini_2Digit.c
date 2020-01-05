
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <mpi.h>

/*****************************************************************************
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
  

To compile:
     mpicc -o Crack_2Ini_2Digit Crack_2Ini_2Digit.c -lcrypt
     
  To run 3 processes on this computer:
    mpirun -n 3 ./Crack_2Ini_2Digit
*****************************************************************************/


int n_passwords = 4;

char *encrypted_passwords[] = {

  "$6$KB$5B3KGhhav2/OsufUGxdicHWDBJM7ojVt0DvSVVwcpHL2OiiiBnq.b6hHZ/uYsi9PETZ6XDOW9/xOOrcbIXcfE/",
  "$6$KB$Lsc5GJc6UWLEyVocbQBpezWOUtiLMkAO6MZ9vJY15o7P5jBXl4w7zxwolBkw5Z4jftUo.6aLeQ6gMUBrADh2q0",
  "$6$KB$f9ah72FpJLoNMH31E3Vvz66fzxDXtNprCFNhUlBMReyEesDqdAy50JiatNkOxYdavQl0OFNRgQG16/OauyswG0",
  "$6$KB$P4lZrgM3qOcCPTJVctZl3bzi90fNT7K7I9mErsPYt1MhU.51HeH4QJTxGTm5pQ6o6g4E7QBmr6rn.7VIuWFS90"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void kernel_function1(char *salt_and_encrypted){
  int a, b, num;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(a='A'; a<='M';a++){
    for(b='A'; b<='Z'; b++){
      for(num=0; num<=99; num++){
	printf("Instance 1:");
	sprintf(plain, "%c%c%02d",a, b, num);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void kernel_function2(char *salt_and_encrypted){
  int a, b, num;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
      for(num=0; num<=99; num++){
	printf("Instance 2:");
	sprintf(plain, "%c%c%02d",a, b, num);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

//calculating time
int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {
 struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  int size, rank;
  int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){

      int x;
    
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } 
    else if(rank==1){
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function1(encrypted_passwords[i]);
        }
    }
    else{
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function2(encrypted_passwords[i]);
        }
    }
  }
  MPI_Finalize(); 
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);

  if(rank==0){
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9)); 
  }
  return 0;
}
