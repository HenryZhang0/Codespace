

int most_frequent(int a[], int n) {
    int map[241+241] = {0};
    for (int i = 0; i < n; i++) {
        map[a[i]+241]++;
    }
    int max = 0;
    int max_index = 0;
    for (int i = 0; i < 241+241; i++) {
        if (map[i] >= max) {
            
            max = map[i];
            max_index = i;
        }
    }
    return max_index-241;
}