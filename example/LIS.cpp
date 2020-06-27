class Solution {
public:
    int lengthLIS(vector<int>& nums) {
        if (nums.empty()) return 0;
        vector<int> f(nums.size(), 1) // initial
        int cur = 1;

        for (int j = 1; j < nums.size(); j++) {
            for (int i = 0; i < j; i++) {
                if (nums[i] < nums[j]) {
                    f[j] = max(f[j], f[i] + 1);
                }
            }
            cur = max(cur, f[j]);
        }
        return cur;
    }
};
