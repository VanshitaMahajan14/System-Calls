#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>

extern int errno;

int main(int argc, char *argv[])
{
    int directory = mkdir("Assignment", 0700);
    char *in = argv[1]; // input file name
    // char *append = "1_";
    long long int len = strlen(argv[1]) + strlen("1_") + strlen("Assignment/");
    char *out = (char *)malloc(len); // will store output file name
    strcat(out, "Assignment/");
    strcat(out, "1_");
    strcat(out, argv[1]); // output file name formed

    int fd_input, fd_output;
    fd_input = open(argv[1], O_RDONLY);
    // flags - read only
    fd_output = creat(out, 0644);
    if(fd_input < 0)       //file descriptor < 0 indicates file can't be opened / doesn't exist
    {
        perror("Error");
        exit(0);
    }
    long long int total = lseek(fd_input, 0, SEEK_END);

    long long int chunk;
    if (total < 1000)
        chunk = 5;
    else
        chunk = 1000;

    char *rev = malloc(sizeof(char) * 1000);             // for storing reversed string
    char *orig = malloc(sizeof(char) * 1000);            // for storing original string
    long long int a = lseek(fd_input, -chunk, SEEK_END); // move input file polong long int er to start of first chunk
    //  prlong long int f("ptr pos %d\n", a);
    lseek(fd_output, 0, SEEK_SET);

    long long int n = total / chunk; // number of groups into which bytes can be divided
    long long int m = total % chunk; // remainder bytes
    // printf("%d %d\n", a, b);
    // printf("%d %d\n", n, m);
    // printf("total = %d\n", total);
    long long int progcount;
    long long int iter = 1;
    while (n > 0)
    {
        long long int hey = lseek(fd_input, -iter * chunk, SEEK_END);
        // move polong long int er that many bytes to the left of file end
        read(fd_input, orig, chunk);
        long long int j = chunk - 1;
        for (long long int i = 0; i < chunk; i++)
        {
            rev[i] = orig[j];
            j--;
        }
        // printf("reverse %s\n", rev);
        write(fd_output, rev, chunk);
        n--;
        iter++;
        progcount = lseek(fd_output, 0, SEEK_END);
        // printf("%d\n",progcount);
        long double per;
        if (total != 0)
            per = (long double)(((long double)progcount / (long double)total) * 100);
        else
            per = 100;
        // counting progress as number of bytes written in output file divided by total bytes in input file
        // printf("%Lf\n",per);
        char *sign = "%";
        char *display = (char *)malloc(sizeof(char) * 100);
        sprintf(display, "\r%0.2Lf%s", per, sign);
        write(1, display, strlen(display));
    }
    long long int x = lseek(fd_input, 0, SEEK_SET);
    // sprintf("cur = %d\n", x);

    if (m != 0) // remainder chunks can be read all at once since they are lesser than chunk size
    {           // lseek()
        long long int newchunk = m;
        char *rev = malloc(sizeof(char) * m);
        char *orig = malloc(sizeof(char) * m);
        long long int j = newchunk - 1;
        read(fd_input, orig, newchunk);
        // prlong long int f("chunk read is %s\n",orig);
        for (long long int i = 0; i < newchunk; i++)
        {
            rev[i] = orig[j];
            j--;
        }
        // prlong long int f("rev = %s\n",rev);
        write(fd_output, rev, newchunk);
    }
    progcount = lseek(fd_output, 0, SEEK_END);
    // prlong long int f("%d\n",progcount);
    long double per;
    if (total != 0)
        per = (long double)(((long double)progcount / (long double)total) * 100);
    else
        per = 100;
    // counting progress as number of bytes written in output file divided by total bytes in input file
    // prlong long int f("%Lf\n",per);
    char *sign = "%";
    char *display = (char *)malloc(sizeof(char) * 100);
    sprintf(display, "\r%0.2Lf%s", per, sign);
    write(1, display, strlen(display));

    close(fd_input);   //close input file
    close(fd_output);  //close output file
    return 0;
}
