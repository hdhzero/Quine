#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    int n_vars;
    int seed;

    cin >> n_vars >> seed;

    cout << n_vars << endl;
    n_vars = 1 << n_vars;
    srand(seed);

    for (int i = 0; i < n_vars; ++i) {
        cout << (rand() % 2) << endl;
    }

    return 0;
}
