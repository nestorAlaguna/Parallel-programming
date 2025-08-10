double temp;
for (int row=n-1; row>=0;row--)
    {
    #pragma omp parallel for num_threads(thread_count) default(none) \
    private(col) shared (x, b, a, n, row)
    for (col = row + 1; col < n; col++) {
        temp = A[row][col]*x[col];
    #pragma omp critical
        x[row] -= temp;
    }
    x[row] /= A[row][row];
}

for (row = 0; row < n; row++) 
{
    x[row] = b[row];
}

    for (col = n -1; col >= 0; col--) {
        x[col] /= aAcol][col];
        #pragma omp parallel for num_threads(thread_count) default(none) \
            private(row) shared(x, b, a, n, col)
        for (row = 0; row < col; row++) 
        {
            x[row] -= A[row][col]*x[col];
    }
}