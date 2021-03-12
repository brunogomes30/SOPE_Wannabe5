#include "../include/auxXmod.h"

u_int8_t getFlags(int nargs, char *args[])
{
    u_int8_t flags = 0;
    for (int i = 1; i < nargs - 2; i++)
    {
        char *flagStr = args[i];
        if (flagStr[0] != '-' || flagStr[2] != 0)
        {
            fprintf(stderr, "Invalid flag %s", flagStr);
            return -1;
        }
        switch (flagStr[1])
        {
        case 'v':
            flags |= 0b100;
            break;
        case 'c':
            flags |= 0b010;
            break;
        case 'R':
            flags |= 0b001;
            break;
        default:
            printf("Invalid flag %s", flagStr);
            return -1;
        }
    }
    return flags;
}

void getSymbolic(mode_t mode, char *output)
{
    char *symbols = "rwx";

    int i = 8, j = 2;
    while (mode)
    {
        if (mode & 1)
        {
            output[i--] = symbols[j];
        }
        else
        {
            output[i--] = '-';
        }
        mode = mode >> 1;
        j--;
        if (j < 0)
            j = 2;
    }
    output[9] = 0;
}

long timedifference_msec(XmodData *processData)
{
    struct timeval t1;
    gettimeofday(&t1, NULL);

    return (t1.tv_sec - processData->startTime.tv_sec) * 1000 + (t1.tv_usec - processData->startTime.tv_usec) / 1000;
}