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
#include "./common/symbol-table.h"

int main()
{
    
    program.scanner = init_scanner();

    program.parser = init_parser(program.scanner);
    
    program.parser->parse(program.parser);

    program.parser->free(program.parser);

    return 0;
}
