#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm.

  Compile with:
    cc -o Three-Initial-Two-Digit Three-Initial-Two-Digit.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./Three-Initial-Two-Digit > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$hY3fOs9ecbMazkkV2AINuCI9UrOM7K2EtoZG7n1xx0xh4..WziZRq5SXOMzgzMx6wCPr6mtBzBhdUSiykJT0g0",
  "$6$KB$U96e8on5TTv9uUuyPDXeXeNUrNQEFQ8MP8xCBGPM4Sh8o.B47L87RAcewN3Y1t.GINNjLv8tFjOXTmEbsSezG/",
  "$6$KB$sMNSMCvmYDYpINGm0hW8SbEmg97A//v5ipbdYfuCVISPfAE0armSK5rJNMv80G7rHg7hCdwjiUwoyoLnLxnEN0",
  "$6$KB$HXJSTbgpOEr5fT6ONJPALNSIcrAOjVJZerQrZrE1UenRnczH0R9lqhmlsYAhJyRuMInFP3nlvqQvOSah2I1sV1"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted){
  int x, y, z, p;   // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(p='A'; p<='Z'; p++){
      	for(z=0; z<=99; z++){
			sprintf(plain, "%c%c%c%02d", x, y, p, z); 
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
	int i;
	struct timespec start, finish;
	long long int time_elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
  
	for(i=0;i<n_passwords;i<i++) {
    	crack(encrypted_passwords[i]);
  	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));

  return 0;
}
