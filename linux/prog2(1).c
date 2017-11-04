#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
void writefile(size_t fd2,size_t fd){
char buf,buf2;
size_t remaining_bytes=read(fd2,&buf,1);
size_t remaining_bytes2=read(fd,&buf2,1);
lseek(fd,-1,1);
lseek(fd2,-1,1);
int bytes_written = write(fd, &buf, remaining_bytes);
if (bytes_written == -1) {
perror("Write");
}
int bytes_written2 = write(fd2, &buf2, remaining_bytes2);
if (bytes_written2 == -1) {
perror("Write");
}
if (bytes_written == 0||bytes_written2 == 0) {
perror("Not enough bytes");
}
}
int fileswap(char** argv)
{
   size_t fd,fd2;


   fd=open(argv[1],O_RDWR | O_CREAT);
   if(-1 == fd)
   {
      perror("Open");
   }

   fd2=open(argv[2],O_RDWR | O_CREAT);
   if(-1 == fd2)
   {
      perror("Open");
   }
   int length1 = lseek(fd, 0, SEEK_END);
   int length2 = lseek(fd2, 0, SEEK_END);
   int length = MIN(length1,length2);
   lseek(fd,0,0);
   lseek(fd2,0,0);
   for(int i=0;i<length;i++){
   writefile(fd,fd2);
   }

   close(fd);
   close(fd2);
   return 0;
}
