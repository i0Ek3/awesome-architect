// 
//

void selectsort(int *a) {
    for (int i = 0; i < a.length(); ++i) {
        int min = i;
        for (int j = 0; j < a.length(); ++j) {
            min = a[j] < a[min] ? j : min;
        }
        swap(a[i], a[min]);
    }
}
