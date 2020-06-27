class Solution {
public:
    int maxProduct(vector<int>& nums) {
        maxP = nums[0];
        minP = nums[0];
        retP = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            int tmp = maxP;
            maxP = max(max(nums[i] * maxP, nums[i]), nums[i] * minP);
            minP = min(min(nums[i] * tmp, nums[i]), nums[i] * minP);
            retP = max(maxP, minP);
        }
        return minP;
    }
};

