#include <bits/stdc++.h>
using namespace std;

static inline vector<long long> extract_ints(const string &s) {
    vector<long long> vals;
    long long sign = 1, num = 0; bool in = false;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '-' && (i + 1 < s.size()) && isdigit(static_cast<unsigned char>(s[i + 1]))) {
            if (in) { vals.push_back(sign * num); num = 0; }
            sign = -1; in = false; // will set true when next digit comes
        } else if (isdigit(static_cast<unsigned char>(c))) {
            num = (in ? num * 10 : 0) + (c - '0');
            in = true;
        } else {
            if (in) { vals.push_back(sign * num); num = 0; in = false; sign = 1; }
            else { sign = 1; }
        }
    }
    if (in) vals.push_back(sign * num);
    return vals;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read all input
    std::string input, line; input.reserve(1<<16);
    while (std::getline(cin, line)) {
        input += line;
        input.push_back('\n');
    }
    if (input.empty()) return 0;

    vector<long long> root;
    long long cnt = -1;

    // Try to parse bracket array for root
    size_t lb = input.find('[');
    size_t rb = input.find(']', lb == string::npos ? 0 : lb + 1);
    if (lb != string::npos && rb != string::npos && rb > lb) {
        string inside = input.substr(lb + 1, rb - lb - 1);
        vector<long long> arr = extract_ints(inside);
        if (!arr.empty()) root = std::move(arr);
        // Try parse cnt by key label
        std::regex r("cnt\\s*=\\s*(-?\\d+)");
        std::smatch m;
        if (std::regex_search(input, m, r)) {
            try { cnt = stoll(m[1].str()); } catch (...) {}
        }
        if (cnt < 0) {
            // Fallback: integers outside bracket, take last as cnt
            string outside = input.substr(0, lb) + input.substr(rb + 1);
            auto ints = extract_ints(outside);
            if (!ints.empty()) cnt = ints.back();
        }
    } else {
        // No bracket form, fallback to generic integer extraction
        auto ints = extract_ints(input);
        if (ints.size() >= 2) {
            // Heuristic: if first int equals count of following root numbers (excluding final cnt)
            // Pattern: n, n numbers, cnt
            long long n = ints[0];
            if (n >= 0 && static_cast<size_t>(n) + 2 == ints.size()) {
                root.assign(ints.begin() + 1, ints.begin() + 1 + n);
                cnt = ints.back();
            } else {
                // Default: last is cnt, rest are root values
                root.assign(ints.begin(), ints.end() - 1);
                cnt = ints.back();
            }
        } else if (ints.size() == 1) {
            // Only one number provided; ambiguity, treat as cnt with empty root (will no-op)
            cnt = ints[0];
        }
    }

    if (root.empty() || cnt <= 0 || static_cast<size_t>(cnt) > root.size()) {
        // Nothing sensible to do
        // To be safe, if cnt is valid but root empty, print nothing
        // But try a last-chance: maybe the whole input is the array and cnt=1
        if (!root.empty() && cnt == -1) cnt = 1;
    }

    if (root.empty() || cnt <= 0 || static_cast<size_t>(cnt) > root.size()) {
        return 0;
    }

    // Kth largest in BST equals kth largest among values (unique values)
    // Sort ascending and pick size - cnt index
    sort(root.begin(), root.end());
    long long ans = root[root.size() - static_cast<size_t>(cnt)];
    cout << ans << '\n';
    return 0;
}
