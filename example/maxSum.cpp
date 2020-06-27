class Solution {
public:
    int maxSum(vector<int>& nums) {
        int curMax = nums[0];
        int retMax = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            curMax = max(curMax + nums[i], nums[i]);
            retMax = max(curMax, retMax);
        }
        return retMax;
    }
};
