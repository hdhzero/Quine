#include "QuineMcCluskey.h"

int QuineMcCluskey::hamming_distance(unsigned a, unsigned b) {
    int dist;
    unsigned val;
 
    dist = 0;
    val = a ^ b;
 
    while (val != 0) {
        dist++;
        val &= val - 1;
    }
 
    return dist;
}

int QuineMcCluskey::hamming_distance(string& a, string& b) {
    int dist = 0;

    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) ++dist;
    }

    return dist;
}


void QuineMcCluskey::read_truth_table_from_file(string filename) {
    int row = 0;
    char c;
    ifstream file;

    file.open(filename.c_str());

    file >> n_vars;

    while (file >> c) {
        if (c == '-') {
            dont_cares.push_back(row);
        } else if (c == '1') {
            minterms.push_back(row);
        }

        ++row;
    }

    file.close();
}


string QuineMcCluskey::get_mask_for_group(vector<int>& g) {
    string mask;
    bool fliped;
    int bit;

    for (int j = n_vars - 1; j >= 0; --j) {
        bit = (1 << j) & g[0];
        fliped = false;

        for (int i = 0; i < g.size(); ++i) {
            if (bit != ((1 << j) & g[i])) {
                mask += '-';
                fliped = true;
                break;
            }
        }

        if (!fliped) {
            if (bit) {
                mask += '1';
            } else {
                mask += '0';
            }
        }
    }

    return mask;
}

void QuineMcCluskey::join(vector<int>& a, vector<int>& b) {
    for (int i = 0; i < b.size(); ++i) {
        a.push_back(b[i]);
    }
}

void QuineMcCluskey::print_groups(vector<vector<int> >& groups) {
    int size;

    for (int i = 0; i < groups.size(); ++i) {
        cout << "(";
        size = groups[i].size();

        for (int j = 0; j < size; ++j) {
            cout << groups[i][j];
            if (j < size - 1) cout << ", ";
        }

        cout << ")  " << get_mask_for_group(groups[i]) << "\n";
    }
}

void QuineMcCluskey::add_to_group(vector<vector<int> >& g, vector<int>& e) {
    bool flag = true;

    for (int i = 0; i < g.size(); ++i) {
        if (e.size() != g[i].size()) {
            continue;
        }

        if (g[i] == e) {
            flag = false;
        }
    }

    if (flag) {
        g.push_back(e);
    }
}

void QuineMcCluskey::first_combination() {
    vector<vector<int> > new_groups;
    vector<vector<int> > old_groups;
    vector<int> minterms_tmp;
    vector<int> tmp;
    string a;
    string b;
    bool combined;
    bool grouped;

    for (int i = 0; i < minterms.size(); ++i) {
        minterms_tmp.push_back(minterms[i]);
    }

    for (int i = 0; i < dont_cares.size(); ++i) {
        minterms_tmp.push_back(dont_cares[i]);
    }

    sort(minterms_tmp.begin(), minterms_tmp.end());

    for (int i = 0; i < minterms_tmp.size(); ++i) {
        tmp.clear();
        tmp.push_back(minterms_tmp[i]);
        old_groups.push_back(tmp);
    }

    combined = true;

    while (combined) {
        vector<bool> visited;

        combined = false;
        visited.resize(old_groups.size());
        for (int i = 0; i < visited.size(); ++i) visited[i] = false;

        for (int i = 0; i < old_groups.size(); ++i) {
            grouped = false;

            for (int j = i + 1; j < old_groups.size(); ++j) {
                a = get_mask_for_group(old_groups[i]);
                b = get_mask_for_group(old_groups[j]);

                if (hamming_distance(a, b) == 1) {
                    tmp.clear();
                    join(tmp, old_groups[i]);
                    join(tmp, old_groups[j]);
                    sort(tmp.begin(), tmp.end());
                    add_to_group(new_groups, tmp);
                    combined = true;
                    visited[i] = true;
                    visited[j] = true;
                    grouped = true;
                }
            }

            if (!visited[i]) {
                add_to_group(prime_implicants, old_groups[i]);
            }
        }

        print_groups(new_groups);

        old_groups.clear();
        old_groups = new_groups;
        new_groups.clear();

        cout << "-------\n";
    }

    cout << "Prime implicants\n";
    print_groups(prime_implicants);
}

void QuineMcCluskey::find_essential_implicants() {
    int count;
    int mterm;
    int index;
    bool extracted;
    vector<int>::iterator b;
    vector<int>::iterator e;
    vector<int> minterms_tmp;
    vector<vector<int> > prime_implicants_tmp;

    extracted = true;
    minterms_tmp = minterms;
    prime_implicants_tmp = prime_implicants;

    while (extracted) {
        extracted = false;

        for (int i = 0; i < minterms_tmp.size(); ++i) {
            count = 0;
            mterm = minterms_tmp[i];

            for (int j = 0; j < prime_implicants_tmp.size(); ++j) {
                b = prime_implicants_tmp[j].begin();
                e = prime_implicants_tmp[j].end();

                if (find(b, e, mterm) != e) {
                    ++count;
                    index = j;
                }
            }

            if (count == 1) {
                extracted = true;
                add_to_group(essential_implicants, prime_implicants_tmp[index]);
            }
        }

        if (extracted) {
            count = 0;

            cout << "remains: ";
            print_groups(prime_implicants_tmp);

            cout << "\n";
            for (int i = 0; i < essential_implicants.size(); ++i) {
                for (int j = 0; j < essential_implicants[i].size(); ++j) {
                    b = minterms_tmp.begin();
                    e = minterms_tmp.end();
                    b = find(b, e, essential_implicants[i][j]);

                    if (b != e) {
                        int elem = *b;
                        bool flag = true;
                        minterms_tmp.erase(b);

                        while (flag) {
                            flag = false;

                            for (int k = 0; k < prime_implicants_tmp.size(); ++k) {
                                b = prime_implicants_tmp[k].begin();
                                e = prime_implicants_tmp[k].end();

                                b = find(b, e, elem);

                                if (b != e) {
                                    prime_implicants_tmp[k].clear();
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }

            cout << "Minterms: ";
            for (int i = 0; i < minterms_tmp.size(); ++i) {
                cout << "{" << minterms_tmp[i] << "}";
            }
            cout << endl;
        }
    }
}

void QuineMcCluskey::debug() {
    int size;
    first_combination();
    find_essential_implicants();

    cout << "Essential implicants:\n";
    print_groups(essential_implicants);
}
