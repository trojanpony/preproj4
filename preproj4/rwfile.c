/*
* Auth: Micah Crenwelge
* Date: 11/5/2022
* Desc: A program that takes file name/s from the command line and copies the contents of the file exactly as they are to a new file using system calls
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define BUFFSIZE (100*1024*1024)

int fd, new_fd, n, m, file_cntr;
char *buf = NULL;
char fname[100];

void cleanup(void) {
  if (buf != NULL) {
    free(buf);
  }
  if (fd <= 1) {
    close(fd);
    fd = -1;
  }
}

void SIGINT_handler( int sig ) {

   /* Issue a message */
   fprintf( stderr, "Whoops!  Program interrupted!\n" );
   
   /* Cleanup */
   cleanup();

   /* Terminate the program HERE */
   exit( EXIT_FAILURE );

}

int main(int argc, char *argv[])
{

  /* allocate buffer */
  buf = (char *)malloc(BUFFSIZE);

  /* Make sure buffer was allocated */
  if (buf == NULL) {
    fprintf(stderr, "Error: could not allocate memory\n");
    exit(1);
  }

  /* Set up signal handler */
  signal( SIGINT, SIGINT_handler );

  /* set file counter to 1 */
  file_cntr = 1;

  /* loop through all files */
  while (file_cntr < argc) {

  /* Open file for in read only mode */
  fd = open(argv[file_cntr], O_RDONLY);

  /* Make sure file was opened */
  if (fd < 0) {
    fprintf(stderr, "Error: could not open file %s\n", argv[1]);
    cleanup();
    exit(1);
  }

  /* Read file into buffer */
  n = read(fd, buf, BUFFSIZE);
  if (n < 0) {
    fprintf(stderr, "Error: could not read file %s\n", argv[1]);
    cleanup();
    exit(1);
  }


  /* Close file */
  close(fd);

  /* Create new file */
  sprintf(fname, "file-%02d.dat", file_cntr);
  new_fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC,  S_IRUSR | S_IWUSR);
  
  /* Make sure file was created */
  if (new_fd < 0) {
    fprintf(stderr, "Error: could not create file %s\n", fname);
    cleanup();
    exit(1);
  }

  /* Write buffer to new file */
  m = write(new_fd, buf, (size_t) n);
  if (m != n) {
    fprintf(stderr, "Error: could not write to file %s\n", fname);
    cleanup();
    exit(1);
 
  /* Close new file */
  close(new_fd);
  }
  file_cntr++;
  }

/* Cleanup */
cleanup();

/* Exit */
return EXIT_SUCCESS;
}
  
