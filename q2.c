#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>

#define size 1e5

// fd1 = fd for input file
// fd2 = fd for output file

extern int errno;

int main(int argc, char *argv[])
{   


    int directory = mkdir("Assignment", 0700);
    char *in = argv[1]; // input file name
    // char *append = "1_";
    long long int len = strlen(argv[1]) + strlen("1_") + strlen("Assignment/");
    char *out = (char *)malloc(len); // will store output file name
    strcat(out, "Assignment/");
    strcat(out, "2_");
    strcat(out, argv[1]); // output file name formed

    int fd1, fd2;
    fd1 = open(argv[1], O_RDONLY | O_CREAT);
    // flags - read only, create
    fd2 = creat(out,0644);
    // flags - create, read/write , truncate
    if(fd1 < 0)
    {
        perror("Error");
        exit(0);
    }
    char *x = argv[2];
    char *y = argv[3];

    int start = atoi(x); // start index
    int end = atoi(y);   // end index

    int total = lseek(fd1, 0, SEEK_END); // total bytes

    int part1 = start;           // bytes in first part
    int part2 = end - start + 1; // bytes in second part
    int part3 = total - end - 1; // bytes in third part

    //printf("%d %d %d %d\n", total, part1, part2, part3);
    
    //setting chunks size as per bytes contained in that part
    int chunk1, chunk2, chunk3;
    if (part1 < 1000)
        chunk1 = 5;
    else
        chunk1 = size;
    if (part2 < 1000)
        chunk2 = 5;
    else
        chunk2 = size;
    if (part3 < 1000)
        chunk3 = 5;
    else
        chunk3 = size;

    int n, m;
    n = part1 / chunk1;
    m = part1 % chunk1;
    int iter;

    char *orig1 = (char *)malloc(sizeof(char) * chunk1);  //store original string read from start index to beginning of file
    char *rev1 = (char *)malloc(sizeof(char) * chunk1);   //store reversed string
    int index = lseek(fd1, start, SEEK_SET);             //index set at start
    //printf("index is %d\n", index);
    
    //INITIAL REVERSED PART

    iter = 1;
    while (n > 0)
    {
        int ind = lseek(fd1, -iter * chunk1, SEEK_CUR); //go iteration*chunks bytes back everytime
        //printf("index after each iter = %d\n", ind);
        read(fd1, orig1, chunk1);
        int j = chunk1 - 1;
        for (int i = 0; i < chunk1; i++)
        {
            rev1[i] = orig1[j];
            j--;
        }
        //printf("%s", rev1);
        write(fd2, rev1, chunk1);
        n--;
        iter++;
    }
    if (m != 0)  //remainder bytes
    {
        char *rem = (char *)malloc(sizeof(char) * m);
        char *rev = (char *)malloc(sizeof(char) * m);
        lseek(fd1, 0, SEEK_SET);
        read(fd1, rem, m);
        int j = m - 1;
        for (int i = 0; i < m; i++)
        {
            rev[i] = rem[j];
            j--;
        }
        write(fd2, rev, m);
    }

    int cur = lseek(fd1, start, SEEK_SET);   // set cursor at start index
    //printf("current is %d\n", cur);
    

    //NON - REVERSED PART

    n = part2 / chunk2;
    m = part2 % chunk2;
    iter = 1;

    char *buf = (char *)malloc(sizeof(char) * chunk2);

    while (n > 0)     //keep reading till proper chunks exist
    { 
        read(fd1, buf, chunk2);
        write(fd2, buf, chunk2);
        iter++;
        n--;
    }
    if (m > 0)     //if any remainder bytes present, simply count from current index
    {
        char *rembuf = (char *)malloc(sizeof(char) * m);
        read(fd1, rembuf, m);
        write(fd2, rembuf, m);
    }

    //  lseek(fd1,0,SEEK_END);

    //FINAL REVERSED PART

    n = part3 / chunk3;
    m = part3 % chunk3;
    iter = 1;
    char *orig3 = (char *)malloc(sizeof(char) * chunk3);
    char *rev3 = (char *)malloc(sizeof(char) * chunk3);
    while (n--)
    {
        lseek(fd1, -iter * chunk3, SEEK_END);
        read(fd1, orig3, chunk3);
        int j = chunk3 - 1;
        for (int i = 0; i < chunk3; i++)
        {
            rev3[i] = orig3[j];
            j--;
        }
        write(fd2, rev3, chunk3);
        iter++;
    }
    lseek(fd1, end + 1, SEEK_SET);  //set at beginning of second reversed portion i.e. end index.(for remainder bytes, if any)
    if (m > 0)
    {
        char *rem = (char *)malloc(sizeof(char) * m);
        char *rev = (char *)malloc(sizeof(char) * m);
        // lseek(fd1,0,SEEK_SET);
        read(fd1, rem, m);
        int j = m - 1;
        for (int i = 0; i < m; i++)
        {
            rev[i] = rem[j];
            j--;
        }
        write(fd2, rev, m);
    }

    close(fd1);
    close(fd2);
    return 0;
}
