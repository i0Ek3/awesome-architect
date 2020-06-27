const double N = 0.0000000001;

double sqrt2() {
    double low = 1.4, high = 1.5;
    double mid = (low + high) / 2;

    while (high - low > N) {
        if (mid * mid > 2) high = mid;
        else low = mid;
        mid = (low + high) / 2;
    }
    return mid;
}
