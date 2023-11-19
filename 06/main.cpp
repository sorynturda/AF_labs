#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unordered_map>
#include <vector>

struct nod {
    int k;
    nod **copii;
    int marime = 0;
};

struct nod_b {
    int k;
    nod_b *copil;
    nod_b *frate;
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

// void afisare_prietenoasa(nod_b *radacina, int spatii) {
//     //pentru R3
//     char s[spatii + 1];
//     memset(s, ' ', spatii);
//     s[spatii] = '\0';
//     printf("%s%d\n", s, radacina->k);
//     for (int i = 0; i < radacina->marime; i++) {
//         afisare_prietenoasa(radacina->copii[i], spatii + 4);
//     }
// }

nod *creare(int k, int n) {
    nod *p = new nod;
    p->k = k;
    p->marime = n;
    p->copii = (nod**)malloc(n * sizeof(nod*));
    return p;
}

nod_b *creare(int k) {
    nod_b *p = new nod_b;
    p->k = k;
    p->copil = NULL;
    p->frate = NULL;
    return p;
}

nod *construire(std::vector<std::vector<int>> &a, int parinte) {
    //pentru T1
    nod *radacina = creare(parinte, a[parinte - 1].size());
    for (int i = 0; i < a[parinte - 1].size(); i++)
        radacina->copii[i] = construire(a, a[parinte - 1][i]);
    return radacina;
}

nod_b *construire(nod *radacina_veche) {
    //pentru T2
    nod_b *radacina = creare(radacina_veche->k);
    if (radacina_veche->marime) {
        radacina->copil = construire(radacina_veche->copii[0]);
        for (int i = 1; i < radacina_veche->marime; i++) {
            radacina->frate = construire(radacina_veche->copii[i]);
            radacina = radacina->frate;
        }
    }
    return radacina;
}

void demo() {
    int p[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    // int p[] = { -1, 1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(p) / sizeof(p[0]);
    int radacina_;
    std::vector<std::vector<int>>a(n, std::vector<int>(0));
    for (int i = 0; i < n; i++)
        if (p[i] != -1)
            a[p[i] - 1].emplace_back(i + 1);
        else
            radacina_ = i + 1;
    nod *radacina = construire(a, radacina_);
    nod_b *radacina_noua = construire(radacina);
    // afisare_prietenoasa(p, n, -1, 0);
    // puts("");
    // afisare_prietenoasa(radacina, 0);
    std::cout<<radacina_noua->copil->copil->frate->k;
}

int main() {
    demo();
    return 0;
}
