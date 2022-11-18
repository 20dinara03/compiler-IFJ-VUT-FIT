/*
 * Welcome to IFJ/IAL 2022/2023 project
 *
 * authors:
 *      Oleksandr Turytsia  | xturyt00 | team-leader
 *      Mark Barzali        | xbarza00
 *      Dinara Garipova     | xgarip00
 *      Kateryna Zdebska    | xzdebs00
 */
#include <stdio.h>
#include "./main.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        // TODO: separate function, move exit code to error.{c,h}
        fprintf(stderr, "Wrong number of arguments\n");
        return EXIT_FAILURE;
    }

    // checking file
    if ((program.src = fopen(argv[1], FILEMODE)) == NULL)
    {
        // TODO: separate function, move exit code to error.{c,h}
        fprintf(stderr, "Cannot open file\n");
        return EXIT_FAILURE;
    }

    program.scanner = init_scanner();
    program.parser = init_parser(program.scanner);

    program.parser->parse(program.parser);

    program.parser->free(program.parser);

    fclose(program.src);
    return 0;
}
