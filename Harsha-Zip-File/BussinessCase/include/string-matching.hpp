#ifndef STRING_MATCHING_HPP
#define STRING_MATCHING_HPP

#include <string>

// Checks if the strings match.
bool str_match(std::string s, std::string pattern, int start = 0) {
    int n = s.length(), m = pattern.length();
    if (n < m + start)
        return false;
    for (int i = start; i <= n - m; i++) {
        int j = 0;
        while (j < m and s[i + j] == pattern[j])
            j++;
        if (j == m)
            return true;
    }
    return false;
}

#endif