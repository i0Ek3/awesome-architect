int partition(vector<int> &n, int low, int up) {
    int pivot = n[up];
    int i = low - 1;
    for (int j = low; j < up; j++) {
        if (n[j] <= pivot) {
            i++;
            swap(n[i], n[j]);
        }
    }
    swap(n[i+1], n[up]);
    return i+1;
}

void quickSort(vector<int> &n, int low, int up) {
    if (low < up) {
        int mid = partition(n, low, up);
        quickSort(n, low, mid-1);
        quickSort(n, mid+1, up);
    }
}



void qsort(vector<int> &v, int left, int right) {
    if (left < right) {
        int key = v[left];
        int low = left;
        int high = right;
        while (low < high) {
            while (low < high && v[high] >= key) {
                high--;
            }
            swap(v[low], v[high]);
            while (low < high && v[low] <= key) {
                low++;
            }
            swap(v[low], v[high]);
        }
        qsort(v, left, low-1);
        qsort(v, low+1, right);
    }
}
