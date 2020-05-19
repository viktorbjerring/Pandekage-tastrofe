#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

char buffer[90000];

/*int main() {
   int fp;
   fp = open("../flaskServer_v1/pancake_status.txt", O_RDWR);
   read(fp, buffer, 90000);
   while(1){
      read(fp, buffer, 90000);
      strcat(buffer, "C-fil\n");
      write(fp, "C-fil\n", strlen("C-fil\n"));
      sleep(1);
   }
   
   close(fp);
}*/

int main(int argc, char **argv){
   return 1;
}