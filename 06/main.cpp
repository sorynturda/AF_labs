#include <stdio.h>
#include <time.h>
#include <string.h>

struct nod {
    nod *copii;
    int marime;
};

void afisare_prietenoasa_r1(int p[], int n, int parinte, int spatii) {
    char s[spatii + 1];
    memset(s, ' ', spatii);
    s[spatii] = '\0';
    for (int i = 0; i < n; i++) {
        if (p[i] == parinte) {
            printf("%s%d\n", s, i + 1);
            afisare_prietenoasa_r1(p, n, i + 1, spatii + 4);
        }
    }
}

void demo() {
    int p[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int n = sizeof(p) / sizeof(p[0]);
    afisare_prietenoasa_r1(p, n, -1, 0);
}

int main() {
    clock_t begin = clock();
    demo();
    clock_t end = clock();
    // printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
