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
#include "queue.h"
#include "token.h"


extern program_t program; 

void free_program(){
    free_queue(program.queue, free_token);
}