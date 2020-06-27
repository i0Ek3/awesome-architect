class Solution {
public:
    void r(vector<char>& s) {
        std::reverse(s.begin(), s.end());
        for (int i = 0, j = 0; i < s.size(); i = j + 1) {
            for (j = i; j < s.size(); ++j) {
                if (s[j] == ' ') break;
            }
            std::reverse(s.begin() + i, s.begin() + j);
        }
    }
};
