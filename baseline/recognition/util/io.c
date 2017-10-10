#include <stdlib.h>
#include <stdio.h>

int **readImgs(int imgSize, int imgSetSize) {
    FILE *fp;
    fp = fopen("./imgs.txt", "r");
    int val;
    int **result = malloc(sizeof(int *) * imgSetSize);
    for (int i = 0; i < imgSetSize; i++) {
        result[i] = malloc(sizeof(int) * imgSize * imgSize);
        for (int j = 0; j < imgSize * imgSize; j++) {
            fscanf(fp, "%d", &val);
            result[i][j] = val;
        }
    }
    fclose(fp);
    return result;
}

int *readImg(int imgSize, char path[]) {
    FILE *fp;
    fp = fopen(path, "r");
    int *result = malloc(sizeof(int) * imgSize * imgSize);
    for (int i = 0; i < imgSize * imgSize; i++) {
        int val;
        fscanf(fp, "%d", &val);
        result[i] = val;
    }
    fclose(fp);
    return result;
}

double **readCplxMatrix(int r, int c, char path[]) {
    FILE *fp;
    fp = fopen(path, "r");
    double **result = malloc(sizeof(double *) * r);
    for (int i = 0; i < r; i++) {
        result[i] = malloc(sizeof(double) * c * 2);
        for (int j = 0; j < c * 2; j++) {
            int val;
            fscanf(fp, "%d", &val);
            result[i][j] = val;
        }
    }
    return result;
}

void storeVec(int *v, int size, char path[]) {
    FILE *fp;
    fp = fopen(path, "w");
    for (int i = 0; i < size * size; i++) {
        fprintf(fp, "%d\n", v[i]);
    }
    fclose(fp);
}

void storeVec_double(double *v, int size, char path[]) {
    FILE *fp;
    fp = fopen(path, "w");
    for (int i = 0; i < size * size; i++) {
        fprintf(fp, "%.3f\n", v[i]);
    }
    fclose(fp);
}
