#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

char buffer[90000];

int main() {
   int fp;
   fp = open("../flaskServer/pancake_status.txt", O_RDWR);
   read(fp, buffer, 90000);
   write(fp, buffer, strlen(buffer));
   while(1){
      strcat(buffer, "C-fil\n");
      write(fp, "C-fil\n", strlen("C-fil\n"));
      sleep(1);
   }
   
   close(fp);
}