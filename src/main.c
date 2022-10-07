/*
 * Welcome to IFJ/IAL 2022/2023 project
 *
 * authors:
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include "main.h"
#include "./common/errors.c"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong number of arguments\n");
        return EXIT_FAILURE;
    }

    FILE *fp; // file pointer

    // checking file
    if ((fp = fopen(argv[1], FILEMODE)) == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", argv[1]);

    return 0;
}
