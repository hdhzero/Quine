#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
using namespace std;

class QuineMcCluskey {
    private:
        int n_vars;
        vector<int> minterms;
        vector<int> dont_cares;
        vector<vector<int> > sorted_minterms;
        vector<vector<int> > prime_implicants;

    public:
        void read_truth_table_from_file(string filename);
        void debug();

    private:
        int ones_count(int a);
        int hamming_distance(unsigned a, unsigned b);
        int hamming_distance(string& a, string& b);
        string get_mask_for_group(vector<int>& g);
        bool can_join(vector<int>& a, vector<int>& b);
        void join(vector<int>& a, vector<int>& b);
        void print_groups(vector<vector<int> >& groups);
        void group_by_numbers_of_ones();
        void add_to_group(vector<vector<int> >& g, vector<int>& e);
        void first_combination();
};


