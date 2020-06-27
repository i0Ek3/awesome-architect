// cocktailsort
//
// 改进的冒泡排序，同时从两边进行冒泡，因为将最小的数也冒泡到了前面，所以较原始的冒泡排序更为高效。
//
//


void cock_tail_sort(int *a) {
    int left = 0, right = a.length()-1;
    while (left < right) {
        for (int i = left; i < right; ++i) {
            if (a[i] > a[i+1]) swap(a[i], a[i+1]);
        }
        right--;
        for (int i = right; i > left; --i) {
            if (a[i] < a[i-1]) swap(a[i], a[i-1]);
        }
        left++;
    }
}
