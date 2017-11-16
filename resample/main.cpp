#include <stdio.h>
#include "./resample.h"

int main() {
    FILE *fp;
    fp = fopen("../data/input.txt", "r");
    int r, c;
    fscanf(fp, "%d", &r);
    fscanf(fp, "%d", &c);

    int input[r * c];
    int dest[400];
    for (int i = 0; i < r * c; i++) {
        int pixel;
        fscanf(fp, "%d", &pixel);
        input[i] = pixel;
    }
    fclose(fp);

    resample(r, c, input, dest);

    fp = fopen("../data/result.txt", "w");
    for (int i = 0; i < 400; i++) {
        fprintf(fp, "%d\n", dest[i]);
    }
    fclose(fp);

    return 0;
}
