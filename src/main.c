/*
 * Welcome to IFJ/IAL 2022/2023 project
 *
 * authors:
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include "./main.h"
#include "./common/error.h"
#include "./scanner/realscanner.h"
#include "./common/symbol-table.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong number of arguments\n");
        return EXIT_FAILURE;
    }

    // checking file
    if ((program.src = fopen(argv[1], FILEMODE)) == NULL)
    {
        fprintf(stderr, "Cannot open file\n");
        return EXIT_FAILURE;
    }

    program.scanner = init_scanner();

    fclose(program.src);

    return 0;
}
