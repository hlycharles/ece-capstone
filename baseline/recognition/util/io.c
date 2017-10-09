#include <stdlib.h>
#include <stdio.h>

unsigned **readImgs(int imgSize, int imgSetSize) {
    FILE *fp;
    fp = fopen("./imgs.txt", "r");
    unsigned val;
    unsigned **result = malloc(sizeof(unsigned *) * imgSetSize);
    for (int i = 0; i < imgSetSize; i++) {
        result[i] = malloc(sizeof(unsigned) * imgSize * imgSize);
        for (int j = 0; j < imgSize * imgSize; j++) {
            fscanf(fp, "%d", &val);
            result[i][j] = val;
        }
    }
    fclose(fp);
    return result;
}

void storeVec(unsigned *v, int size, char path[]) {
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
