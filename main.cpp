#include "QuineMcCluskey.h"

int main(int argc, char** argv) {
    QuineMcCluskey quine;

    quine.read_truth_table_from_file(argv[1]);
    quine.debug();

    return 0;
}
