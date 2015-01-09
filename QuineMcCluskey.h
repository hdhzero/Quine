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
        vector<vector<int> > essential_implicants;

    public:
        void read_truth_table_from_file(string filename);
        void debug();

    private:
        int hamming_distance(unsigned a, unsigned b);
        int hamming_distance(string& a, string& b);
        string get_mask_for_group(vector<int>& g);
        bool can_join(vector<int>& a, vector<int>& b);
        void join(vector<int>& a, vector<int>& b);
        void print_groups(vector<vector<int> >& groups);
        void add_to_group(vector<vector<int> >& g, vector<int>& e);
        void first_combination();
        void find_essential_implicants();
};


