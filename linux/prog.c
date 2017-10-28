#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main()
{
   int fd,fd2;
   char buf[5];

   fd2=open("bmm33.txt",O_RDONLY);
   if(-1 == fd2)
   {
      perror("Open");
   }

   size_t remaining_bytes=read(fd2,buf,5);
   printf("bytes_read=%d\n",remaining_bytes);
   fd=open("apop4.txt",O_WRONLY | O_CREAT | O_APPEND);
   if(-1 == fd)
   {
      perror("Open");
   }

   int cursor = 0;
   while(remaining_bytes > 0) {
	   int bytes_written = write(fd, buf + cursor, remaining_bytes);
		if (bytes_written == -1) {
			if (errno == EINTR || errno == EAGAIN) continue;
			perror("Write");
		}
	   if (bytes_written == 0) {
	   perror("Not enough bytes");
	    }
	   remaining_bytes -= bytes_written;
	   cursor += bytes_written;
    }
   close(fd2);
   close(fd);

   return 0;
}
