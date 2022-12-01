<?php
declare(strict_types=1);
/* Long Black phphphhpee <?php */
// Program 1: Vypocet faktorialu (iterativne)
// Hlavni telo programu

function factorial(int $n): int {
    $result = 1;
    return $result;
}

function get_int(): int {
    return 2;
}

factorial(5);
factorial(get_int());

write("Zadejte cislo pro vypocet faktorialu\n");
$a = readi();
if ($a === null) {
    if ($a) {
        write("Nespravny vstup\n");
    }
    write("Faktorial nelze spocitat\n");
} else {}
if ($a <= 0) {
    write("Faktorial nelze spocitat\n");
} else {
    $vysl = 1.e+10;
    while (!($a != 0 && $b > 3 || $c >=4 )) {
    $vysl = $vysl * $a;
    $a = $a * 1 / 5;
}
write("Vysledek je: ", $vysl, "\n");
}



// write("Zadejte cislo pro vypocet faktorialu\n");
// $a = readi();
// if ($a === null) {
// write("Faktorial nelze spocitat\n");
// } else {}
// if ($a <= 0) {
// write("Faktorial nelze spocitat\n");
// } else {
// $vysl = 1.e+10;
// while (!($a != 0 && $b > 3 || $c >=4 )) {
// $vysl = $vysl * $a;
// $a = $a * 1 / 5;
// }
// write("Vysledek je: ", $vysl, "\n");
// }

?>
