#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
extern int errno;

#define size 1e5

int main(int num_arg, char *argv[])
{
    if (num_arg != 4)
    {
        char *error = "Input format not followed\n";
        int len = strlen(error);
        write(1, error, len);
        exit(1);
    }
    int fdnew, fdold, fddir;
    fdnew = open(argv[1], O_RDONLY); // file descriptor for newfile
    fdold = open(argv[2], O_RDONLY); // file descriptor for old file

    if (fdnew == -1)
    {
        // char *error = "Input file cannot be opened\n";
        // int len = strlen(error);
        // write(1, error, strlen(error));
        perror("Error");
        exit(0);
    }
    if (fdold == -1)
    {
        perror("Error");
        exit(0);
    }

    struct stat fileperm;

    const char *pathnewfile = argv[1]; // argv[1] is path of newfile
    const char *patholdfile = argv[2]; // argv[2] is path of oldfile
    const char *pathdir = argv[3];     // argv[3] is path of directory

    int check = 1;

    long long int totalnew = lseek(fdnew, 0, SEEK_END); // total bytes in new file
    long long int totalold = lseek(fdold, 0, SEEK_END); // total bytes in old file
    // printf("%d %d\n",totalnew,totalold);
    lseek(fdold, 0, SEEK_SET);
    if (totalnew != totalold)
        check = 0;

    else
    {
        long long int chunk;
        if (totalnew < 1000)
            chunk = 5;
        else
            chunk = size;

        long long int p = totalold / chunk;
        long long int q = totalold % chunk;

        char *readnew = malloc(sizeof(char) * chunk); // string read in reverse direction from newfile
        char *readold = malloc(sizeof(char) * chunk); // string read from starting of oldfile

        long long int iter = 1;

        while (p > 0)
        {
            lseek(fdnew, -iter * chunk, SEEK_END);
            read(fdnew, readnew, chunk);
            read(fdold, readold, chunk);
            // printf("%s %s\n",readnew,readold);
            long long int j = chunk - 1;
            for (long long int i = 0; i < chunk; i++)
            { // printf("%c %c\n",readnew[j],readold[i]);
                if (readnew[j] != readold[i])
                {
                    check = 0;
                    break;
                }
                j--;
            }
            iter++;
            p--;
        }
        long long int x = lseek(fdnew, 0, SEEK_SET);
        if (check == 1 && q > 0) // checking remainder chunks
        {
            long long int newchunk = q;
            char *newread = malloc(sizeof(char) * q);
            char *oldread = malloc(sizeof(char) * q);
            // printf("%s %s\n",newread,oldread);
            read(fdold, oldread, q);
            read(fdnew, newread, q);
            long long int j = q - 1;
            // printf("chunk read is %s\n",orig);
            for (long long int i = 0; i < q; i++)
            {
                // printf("%c %c\n",newread[j],oldread[i]);
                if (newread[j] != oldread[i])
                {
                    check = 0;
                    break;
                }
                j--;
            }
        }
    }
    if (check == 1)
    {
        char *dis = "Whether file contents are reversed in newfile: Yes\n";
        int len = strlen(dis);
        write(1, dis, len);
    }
    else
    {
        char *dis = "Whether file contents are reversed in newfile: No\n";
        int len = strlen(dis);
        write(1, dis, len);
    }
    char *line = "\n";
    // write(1, line, 8);

    if (stat(argv[3], &fileperm) == 0 && S_ISDIR(fileperm.st_mode))
    {
        char *dis = "Directory is created: Yes\n";
        write(1, dis, strlen(dis));
    }
    else
    {
        char *dis = "Directory is created: No\n";
        write(1, dis, strlen(dis));
    }

    // IR_USR - read permisison for user
    // IW_USR - write permission for user
    // IX_USR - execute permisison for user

    // IR_GRP - read permisison for group
    // IW_GRP - write permission for group
    // IX_GRP - execute permisison for group

    // IR_OTH - read permisison for others
    // IW_OTH - write permission for others
    // IX_OTH - execute permisison for others

    // NEWFILE PERMISSIONS

    // stat(argv[1],&fileperm) checks if the given newfile path exist or not
    // if file exists - return 0
    // if file does not exist - return -1
    // The code assumes that if file does not exist, the permission to read, write or execute it does not exist

    if (stat(argv[1], &fileperm) == 0)
    {
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IRUSR)
        {
            char *dis = "User has read permissions on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has read permissions on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IWUSR)
        {
            char *dis = "User has write permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has write permissions on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IXUSR)
        {
            char *dis = "User has execute permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has execute permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IRGRP)
        {
            char *dis = "Group has read permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has read permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IWGRP)
        {
            char *dis = "Group has write permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has write permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IXGRP)
        {
            char *dis = "Group has execute permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has execute permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IROTH)
        {
            char *dis = "Others has read permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has read permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IWOTH)
        {
            char *dis = "Others has write permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has write permission on newfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[1], &fileperm) == 0 && fileperm.st_mode & S_IXOTH)
        {
            char *dis = "Others has execute permission on newfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {

            char *dis = "Others has execute permission on newfile: No\n\n\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
    }
    else
    {
        char *err = "newfile does not exist\n";
        int len = strlen(err);
        write(1, err, len);
    }
    write(1, line, 8);

    // OLDFILE PERMISSIONS
    if (stat(argv[2], &fileperm == 0))
    {
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IRUSR)
        {
            char *dis = "User has read permissions on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has read permissions on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IWUSR)
        {
            char *dis = "User has write permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has write permissions on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IXUSR)
        {
            char *dis = "User has execute permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has execute permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IRGRP)
        {
            char *dis = "Group has read permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has read permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IWGRP)
        {
            char *dis = "Group has write permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has write permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IXGRP)
        {
            char *dis = "Group has execute permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has execute permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IROTH)
        {
            char *dis = "Others has read permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has read permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IWOTH)
        {
            char *dis = "Others has write permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has write permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[2], &fileperm) == 0 && fileperm.st_mode & S_IXOTH)
        {
            char *dis = "Others has execute permission on oldfile: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has execute permission on oldfile: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
    }
    else
    {
        char *err = "oldfile does not exist\n";
        int len = strlen(err);
        write(1, err, len);
    }
    // char *line = "\n";
    write(1, line, 8);

    // DIRECTORY PERMISSIONS
    if (stat(argv[3], &fileperm) == 0)
    {
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IRUSR)
        {
            char *dis = "User has read permissions on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has read permissions on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IWUSR)
        {
            char *dis = "User has write permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has write permissions on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IXUSR)
        {
            char *dis = "User has execute permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "User has execute permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IRGRP)
        {
            char *dis = "Group has read permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has read permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IWGRP)
        {
            char *dis = "Group has write permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has write permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IXGRP)
        {
            char *dis = "Group has execute permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Group has execute permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IROTH)
        {
            char *dis = "Others has read permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has read permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IWOTH)
        {
            char *dis = "Others has write permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has write permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        if (stat(argv[3], &fileperm) == 0 && fileperm.st_mode & S_IXOTH)
        {
            char *dis = "Others has execute permission on directory: Yes\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
        else
        {
            char *dis = "Others has execute permission on directory: No\n";
            int len = strlen(dis);
            write(1, dis, len);
        }
    }
    else
    {
        char *err = "directory does not exist\n";
        int len = strlen(err);
        write(1, err, len);
    }

    return 0;
}
