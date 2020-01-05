#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Data CrackAZ99-With-Data.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$5B3KGhhav2/OsufUGxdicHWDBJM7ojVt0DvSVVwcpHL2OiiiBnq.b6hHZ/uYsi9PETZ6XDOW9/xOOrcbIXcfE/",
  "$6$KB$Lsc5GJc6UWLEyVocbQBpezWOUtiLMkAO6MZ9vJY15o7P5jBXl4w7zxwolBkw5Z4jftUo.6aLeQ6gMUBrADh2q0",
  "$6$KB$f9ah72FpJLoNMH31E3Vvz66fzxDXtNprCFNhUlBMReyEesDqdAy50JiatNkOxYdavQl0OFNRgQG16/OauyswG0",
  "$6$KB$P4lZrgM3qOcCPTJVctZl3bzi90fNT7K7I9mErsPYt1MhU.51HeH4QJTxGTm5pQ6o6g4E7QBmr6rn.7VIuWFS90"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(){
  int i;
  pthread_t t1, t2;
  void *kernel_function_1();
  void *kernel_function_2();
  
  for(i=0; i<n_passwords; i<i++){
	  pthread_create(&t1, NULL, kernel_function_1, encrypted_passwords[i]);
	  pthread_create(&t2, NULL, kernel_function_2, encrypted_passwords[i]);

	  pthread_join(t1, NULL);
	  pthread_join(t2, NULL);
  }
}

void *kernel_function_1(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } //else {
	  //printf(" %-8d%s %s\n", count, plain, enc);
	//}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void *kernel_function_2(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } //else {
	  //printf(" %-8d%s %s\n", count, plain, enc);
	//}
      }
    }
  }
  printf("%d solutions explored\n", count);
}

//Calculating time
int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
{
	long long int ds = finish->tv_sec - start->tv_sec;
	long long int dn = finish->tv_nsec - start->tv_nsec;

	if(dn < 0){
		ds--;
		dn += 1000000000;
	}
	*difference = ds * 1000000000 + dn;
	return !(*difference > 0);
}

int main(int argc, char *argv[]){

	struct timespec start, finish;
	long long int time_elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
    crack();
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));

  return 0;
}