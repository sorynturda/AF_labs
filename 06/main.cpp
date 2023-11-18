#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unordered_map>

struct nod {
    int k;
    nod **copii;
    int marime = 0;
};

void afisare_prietenoasa(int p[], int n, int parinte, int spatii) {
    //pentru R1
    char s[spatii + 1];
    memset(s, ' ', spatii);
    s[spatii] = '\0';
    for (int i = 0; i < n; i++) {
        if (p[i] == parinte) {
            printf("%s%d\n", s, i + 1);
            afisare_prietenoasa(p, n, i + 1, spatii + 4);
        }
    }
}

void afisare_prietenoasa(nod *radacina, int spatii) {
    //pentru R2
    char s[spatii + 1];
    memset(s, ' ', spatii);
    s[spatii] = '\0';
    printf("%s%d\n", s, radacina->k);
    for (int i = 0; i < radacina->marime; i++) {
        afisare_prietenoasa(radacina->copii[i], spatii + 4);
    }
}

nod *creare(int k, int n) {
    nod *p = new nod;
    p->k = k;
    p->marime = n;
    p->copii = (nod**)malloc(n * sizeof(nod*));
    return p;
}

nod *construire_t2(int p[], int n, std::unordered_map <int, int> &H, int parinte) {
    nod *radacina = creare(parinte, H[parinte]);
    int k = 0;
    for (int i = 0; i < n; i++)
        if (p[i] == parinte)
            radacina->copii[k++] = construire_t2(p, n, H, i + 1);
    return radacina;
}

void demo() {
    int p[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    // int p[] = { -1, 1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(p) / sizeof(p[0]);
    std::unordered_map <int, int> H;
    int radacina_;
    for (int i = 0; i < n; i++)
        if (p[i] != -1)
            H[p[i]]++;
        else
            radacina_ = i + 1;
    nod *radacina = construire_t2(p, n, H, radacina_);
    // std::cout << radacina_ << '\n';
    // std::cout << radacina->copii[0]->marime << '\n';
    afisare_prietenoasa(p, n, -1, 0);
    puts("");
    afisare_prietenoasa(radacina, 0);
}

int main() {
    clock_t begin = clock();
    demo();
    clock_t end = clock();
    // printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
