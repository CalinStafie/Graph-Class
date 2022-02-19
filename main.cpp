#include <bits/stdc++.h>
#define NIL -1
#define INFINIT INT_MAX

using namespace std;

typedef pair<int, int> pereche;

struct muchie
{
    int sursa, destinatie, cost;
    muchie (int sursa = 0, int destinatie = 0, int cost = 0)
    {
        this->sursa = sursa;
        this->destinatie = destinatie;
        this->cost = cost;
    }
};

class Graful_meu
{
    int _noduri;
    int _muchii;
    vector<vector<muchie>> _lista_adiacenta;
    bool _orientat, _are_cost;

public:
    Graful_meu(int noduri = 0, int muchii = 0, bool _orientat = false, bool _are_cost = false);

    void citire_graf(istream &fin);
    int getter_numar_noduri();
    int getter_numar_muchii();
    void setter_numar_noduri(int numar_noduri_noi);
    void setter_numar_muchii(int numar_muchii_noi);
    void adaugare_muchie(int x, int y, int c);

    // tema - part 1
    void dfs(int start, bool vizitat[]);
    int componente_conexe();
    vector<int> bfs(int start);
    vector <vector<int>> componente_biconexe();
    vector<vector<int>> componente_tare_conexe();
    vector<vector<int>> muchii_critice();
    vector<int> sortare_topologica();
    bool havel_hakimi(int n, vector<int>&grade);

    // tema - part 2
    vector<int> prim_algoritm(int &cost_total, int nod_start);
    vector<pereche> kruskal_algoritm(int &cost_total);
    static int gaseste_radacina(int nod, vector<int>&tata);
    static void reuniune_radacini(int nod_1, int nod_2, vector<int>&tata);
    vector<int> dijkstra_algoritm(int nod_start);
    vector<int> bellmanford_algoritm(int nod_start);

    // tema - part 3
    vector<vector<long long>> roy_floyd_algoritm(vector<vector<long long>>&mat);
    int diametru_arbore();
    int flux_maxim_edmonds_karp_algoritm(int sursa, int destinatie);

    // tema - part 4
    vector<int> cuplaj_maxim_in_graj_bipartit(int &cuplaj_maxim, int numar_noduri_stanga, int numar_noduri_dreapta);
    vector<int> verificare_si_constructie_ciclu_eulerian(int nod_start);
    int cost_minim_ciclu_hamiltonian();

private:
    void dfs_componente_biconexe(int nod_curent, stack<int>&stiva_noduri, vector<int>&nod_intoarcere, vector<int>&timp_descoperire_nod, vector<vector<int>>&toate_componentele_biconexe);
    void dfs_componente_tare_conexe(int nod_curent, stack<int>&stiva_noduri, vector<bool>&apartine_unei_comp_tare_conexe, vector<int>&nod_intoarcere, vector<int>&timp_descoperire_nod, vector<vector<int>>&toate_componentele_tare_conexe);
    void dfs_muchii_critice(int nod_curent, int parinte, vector<int>&nod_intoarcere, vector<int>&timp_descoperire_nod, vector<vector<int>>&toate_muchiile_critice);
    int bfs_edmonds_karp_algoritm(int sursa, int destinatie, vector<int> &tata, vector<vector<int>>&capacitate_reziduala, vector<vector<int>>&adj);
    bool cauta_cuplaj_bipartit(int nod_stanga, vector<bool>&vizitat, vector<int>&potrivire);
    void ciclu_eulerian_dfs(int nod_start, vector<bool>&vizitat, vector<int>&muchii_ciclu_eulerian);
};

Graful_meu::Graful_meu(int noduri, int muchii, bool orientat, bool are_cost)
{
    _noduri = noduri;
    _muchii = muchii;
    _orientat = orientat;
    _are_cost = are_cost;
    _lista_adiacenta.resize(_noduri + 2);
}
void Graful_meu::citire_graf(istream &fin)
{
    int x, y;
    int cost = 0;
    for(int i = 1 ; i <= _muchii ; ++i)
    {
        fin >> x >> y;
        if(_are_cost)
            fin >> cost;
        _lista_adiacenta[x].push_back(muchie(x, y, cost));
        if(!_orientat)
            _lista_adiacenta[y].push_back(muchie(y, x, cost));
    }
}

int Graful_meu::getter_numar_noduri()
{
    return _noduri;
}

int Graful_meu::getter_numar_muchii()
{
    return _muchii;
}

void Graful_meu::setter_numar_noduri(int numar_noduri_noi)
{
    _noduri = numar_noduri_noi;
}

void Graful_meu::setter_numar_muchii(int numar_muchii_noi)
{
    _muchii = numar_muchii_noi;
}

void Graful_meu::adaugare_muchie(int x, int y, int c)
{
    _lista_adiacenta[x].push_back(muchie(x, y, c));
    ++_muchii;
    if(!_orientat)
    {
        _lista_adiacenta[y].push_back(muchie(y, x, c));
        ++_muchii;
    }
}

// parcurgere DFS
void Graful_meu::dfs(int start, bool vizitat[])
{
    vizitat[start] = 1;
    for(auto nod_vecin : _lista_adiacenta[start])
        if(vizitat[nod_vecin.destinatie] == 0)
            dfs(nod_vecin.destinatie, vizitat);

}

// se va returna numarul de componente conexe, folosindu-ne de functia DFS
int Graful_meu::componente_conexe()
{
    bool vizitat[_noduri + 2] = {0};
    int numar_componente_conexe = 0;
    for(int i = 1 ; i <= _noduri ; ++i)
        if (vizitat[i] == 0)
        {
            dfs(i, vizitat);
            ++numar_componente_conexe;
        }
    return numar_componente_conexe;
}

// parcurgerea BFS
vector<int> Graful_meu::bfs(int start)
{
    bool vizitat[_noduri + 2] = {0};
    queue<int>coada_bfs;
    vector<int> drumuri_minime(_noduri + 2, -1);

    coada_bfs.push(start);
    vizitat[start] = 1;
    drumuri_minime[start] = 0;

    while(!coada_bfs.empty())
    {

        for(auto nod_vecin : _lista_adiacenta[start])
            if(vizitat[nod_vecin.destinatie] == 0)
            {
                coada_bfs.push(nod_vecin.destinatie);
                vizitat[nod_vecin.destinatie] = 1;
                drumuri_minime[nod_vecin.destinatie] = drumuri_minime[start] + 1;
            }
        coada_bfs.pop();
        start = coada_bfs.front();
    }

    return drumuri_minime;
}

// functia va returna fiecare vector de noduri care compun o componenta biconexa, ne ajutam de un DFS modificat
vector<vector<int>> Graful_meu::componente_biconexe()
{
    vector<int>timp_descoperire_nod(_noduri + 2, NIL);
    vector<int>nod_intoarcere(_noduri + 2, NIL);
    vector<vector<int>> toate_componentele_biconexe;
    stack<int>stiva_noduri;

    for(int i = 1; i <= _noduri; ++i)
        if(timp_descoperire_nod[i] == NIL)
            dfs_componente_biconexe(i, stiva_noduri, nod_intoarcere, timp_descoperire_nod, toate_componentele_biconexe);

    return toate_componentele_biconexe;
}

// in functie verificam daca nodul curent este sau nu punct de articulatie(nod critic)
// o idee adaptata pornind de la aceasta: un nod este critic daca are un fiu care are nivelul de intoarcere >= ca nivelul nodului(tata)
void Graful_meu::dfs_componente_biconexe(int nod_curent, stack<int>&stiva_noduri, vector<int>&nod_intoarcere,
                                        vector<int>&timp_descoperire_nod, vector<vector<int>>&toate_componentele_biconexe)
{
    static int timp = 0;
    nod_intoarcere[nod_curent] = timp_descoperire_nod[nod_curent] = ++timp;
    stiva_noduri.push(nod_curent);

    for(auto vecin : _lista_adiacenta[nod_curent])
    {
        if(timp_descoperire_nod[vecin.destinatie] == NIL)
        {
            dfs_componente_biconexe(vecin.destinatie, stiva_noduri, nod_intoarcere, timp_descoperire_nod, toate_componentele_biconexe);

            nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], nod_intoarcere[vecin.destinatie]);
            if(nod_intoarcere[vecin.destinatie] >= timp_descoperire_nod[nod_curent])
            {
                vector<int>componenta_biconexa;
                int nod_stiva = stiva_noduri.top();
                stiva_noduri.pop();
                componenta_biconexa.push_back(nod_stiva);
                while(nod_stiva != vecin.destinatie)
                {
                    nod_stiva = stiva_noduri.top();
                    stiva_noduri.pop();
                    componenta_biconexa.push_back(nod_stiva);
                }
                componenta_biconexa.push_back(nod_curent);

                toate_componentele_biconexe.push_back(componenta_biconexa);
            }
        }
        else
            nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], timp_descoperire_nod[vecin.destinatie]);
    }
}

// functia va returna fiecare vector de noduri care compun o componenta tare conexa, ne ajutam de un DFS modificat
vector<vector<int>> Graful_meu::componente_tare_conexe()
{
    vector<int> timp_descoperire_nod(_noduri + 2, NIL);
    vector<int> nod_intoarcere(_noduri + 2, NIL);
    vector<vector<int>> toate_componentele_tare_conexe;
    stack<int> stiva_noduri;
    vector<bool> apartine_unei_comp_tare_conexe(_noduri + 2, false);

    for(int i = 1; i <= _noduri; ++i)
        if(timp_descoperire_nod[i] == NIL)
            dfs_componente_tare_conexe(i, stiva_noduri, apartine_unei_comp_tare_conexe, nod_intoarcere, timp_descoperire_nod, toate_componentele_tare_conexe);

    return toate_componentele_tare_conexe;
}

// aceasta functie are ca scop determinarea fiecarei radacini a componentelor tare conexe,
// mai precis in if-ul acela unde testam daca nod_intoarcere[vecin] == timp_descoperire_nod[nod_curent]
void Graful_meu::dfs_componente_tare_conexe(int nod_curent, stack<int>&stiva_noduri, vector<bool>&apartine_unei_comp_tare_conexe, vector<int>&nod_intoarcere,
                                        vector<int>&timp_descoperire_nod, vector<vector<int>>&toate_componentele_tare_conexe)
{
    static int timp = 0;
    nod_intoarcere[nod_curent] = timp_descoperire_nod[nod_curent] = ++timp;
    stiva_noduri.push(nod_curent);

    for(auto vecin : _lista_adiacenta[nod_curent])
    {
        if(timp_descoperire_nod[vecin.destinatie] == NIL)
        {
            dfs_componente_tare_conexe(vecin.destinatie, stiva_noduri, apartine_unei_comp_tare_conexe, nod_intoarcere, timp_descoperire_nod, toate_componentele_tare_conexe);

            nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], nod_intoarcere[vecin.destinatie]);
        }
        else
            if(apartine_unei_comp_tare_conexe[vecin.destinatie] == false)
                nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], timp_descoperire_nod[vecin.destinatie]);
    }

    if(nod_intoarcere[nod_curent] == timp_descoperire_nod[nod_curent])
    {
        vector<int>componenta_tare_conexa;
        int nod_stiva = stiva_noduri.top();
        while(nod_stiva != nod_curent)
        {
            componenta_tare_conexa.push_back(nod_stiva);
            apartine_unei_comp_tare_conexe[nod_stiva] = true;
            stiva_noduri.pop();
            nod_stiva = stiva_noduri.top();
        }
        componenta_tare_conexa.push_back(nod_stiva);
        apartine_unei_comp_tare_conexe[nod_stiva] = true;
        stiva_noduri.pop();

        toate_componentele_tare_conexe.push_back(componenta_tare_conexa);
    }
}

// functie ce returneaza toate muchiile critice
vector<vector<int>> Graful_meu::muchii_critice()
{
    vector<vector<int>> _lista_adiacenta(_noduri + 2);
    vector<int>timp_descoperire_nod(_noduri + 2, NIL);
    vector<int>nod_intoarcere(_noduri + 2, NIL);
    vector<vector<int>>toate_muchiile_critice;

    for(int i = 1 ; i <= _noduri ; ++i)
        if(timp_descoperire_nod[i] == -1)
            dfs_muchii_critice(i, -1, nod_intoarcere, timp_descoperire_nod, toate_muchiile_critice);

    return toate_muchiile_critice;
}

// functia este un DFS adaptat care se bazeaza pe urmatoarea idee:
// o muchie este critica daca nivelul de intoarcere(un fel de nivel de intoarcere: nod_intoarcere) al fiului(vecin) > ca nivelul tatalui (nod_curent)
void Graful_meu::dfs_muchii_critice(int nod_curent, int parinte, vector<int>&nod_intoarcere, vector<int>&timp_descoperire_nod,
                                                vector<vector<int>>&toate_muchiile_critice)
{
    static int timp = 0;
    nod_intoarcere[nod_curent] = timp_descoperire_nod[nod_curent] = ++timp;

    for(auto vecin : _lista_adiacenta[nod_curent])
    {
        if(timp_descoperire_nod[vecin.destinatie] == NIL)
        {
            parinte = nod_curent;
            dfs_muchii_critice(vecin.destinatie, parinte, nod_intoarcere,timp_descoperire_nod, toate_muchiile_critice);
            nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], nod_intoarcere[vecin.destinatie]);

            if(nod_intoarcere[vecin.destinatie] > timp_descoperire_nod[nod_curent])
                toate_muchiile_critice.push_back({nod_curent, vecin.destinatie});
        }
        else
            if(parinte != vecin.destinatie)
                nod_intoarcere[nod_curent] = min(nod_intoarcere[nod_curent], timp_descoperire_nod[vecin.destinatie]);
    }
}

// sortarea topologica (metoda de implementare fix ca la curs, NU cea cu un DFS)
vector<int> Graful_meu::sortare_topologica()
{
    int grad_interior[_noduri + 2] = {0};
    vector<int>ordine_topologica;

    for(int i = 1 ; i <= _noduri ; ++i)
        for(auto vecin : _lista_adiacenta[i])
            ++grad_interior[vecin.destinatie];

    int copie_numar_noduri = _noduri;
    while(copie_numar_noduri)
    {
        for(int nod_curent = 1; nod_curent <= _noduri; ++nod_curent)
            if(grad_interior[nod_curent] == 0)
            {
                ordine_topologica.push_back(nod_curent);
                for(auto vecin : _lista_adiacenta[nod_curent])
                    --grad_interior[vecin.destinatie];
                grad_interior[nod_curent] = NIL;
                --copie_numar_noduri;
            }
    }

    return ordine_topologica;
}

// algoritmul lui Havel Hakimi care ne va spune daca se poate forma un graf din vectorul de grade
bool Graful_meu::havel_hakimi(int n, vector<int>&grade)
{
    int suma = 0;
    for(auto element : grade)
    {
        suma += element;
        if(element > n - 1)
            return false;
    }
    if(suma % 2)
       return false;

    while(all_of(grade.begin(), grade.end(), [](int i) {return i == 0;}) == false)
    {
        sort(grade.begin(), grade.end(), greater<int>());
        int valoare_grad = grade[0];
        grade.erase(grade.begin());
        for(int i = 0 ; i < valoare_grad ; ++i)
        {
            --grade[i];
            if(grade[i] < 0)
                return false;
        }
    }
    return true;

}

// algoritmul lui Prim implementat cu priority queue, va returna vectorul de tati si va calcula costul total pe parcursul adaugarii in APM
vector<int> Graful_meu::prim_algoritm(int &cost_total, int nod_start)
{
    priority_queue<pereche, vector<pereche>, greater<pereche>> min_heap;
    vector<int> cost(_noduri + 2, INFINIT);
    vector<int> tata(_noduri + 2, NIL);
    vector<bool> apartine_apm(_noduri + 2, false);
    cost_total = 0;

    min_heap.push({0, nod_start});
    cost[nod_start] = 0;

    while(!min_heap.empty())
    {
        int nod_curent = min_heap.top().second;
        int cost_curent = min_heap.top().first;
        min_heap.pop();

        if(apartine_apm[nod_curent] == true)
            continue;

        cost_total += cost_curent;
        apartine_apm[nod_curent] = true;
        for(auto vecin : _lista_adiacenta[nod_curent])
        {
            int cost_vecin = vecin.cost;
            int nod_vecin = vecin.destinatie;

            if(apartine_apm[nod_vecin] == false and cost[nod_vecin] > cost_vecin)
            {
                cost[nod_vecin] = cost_vecin;
                min_heap.push({cost_vecin, nod_vecin});
                tata[nod_vecin] = nod_curent;
            }
        }
    }

    return tata;
}

// returneaza radacina arborelui in care se afla nodul din argument(nodul curent)
// VARIANTA OPTIMIZATA - legam toate nodurile pe parcursul recursiei de radacina arborelui din care fac parte, adica sa avem
// nodurile intalnite in recursivitate pe un nivel
int Graful_meu::gaseste_radacina(int nod, vector<int>&tata)
{
    if(nod == tata[nod])
        return nod;
    return tata[nod] = gaseste_radacina(tata[nod], tata);
}

//unim radacinile a doi arbori diferiti
void Graful_meu::reuniune_radacini(int nod1, int nod2, vector<int>&tata)
{
    tata[nod1] = tata[nod2];
}

// metoda aplica algoritmul lui Kruskal si calculam costul_total si returnam lista de muchii a arborelui partial de cost minim
vector<pereche> Graful_meu::kruskal_algoritm(int &cost_total)
{
    vector<int>tata(_noduri + 2);
    vector<pair<int, pereche>>lista_muchii;
    vector<pereche>apm;
    cost_total = 0;
    for (int i = 1 ; i <= _noduri ; ++i)
        tata[i] = i;
    for(int i = 1 ; i <= _noduri ; ++i)
        for(auto element : _lista_adiacenta[i])
            lista_muchii.push_back({element.cost, {i, element.destinatie}});
    sort(lista_muchii.begin(), lista_muchii.end());
    for(auto muchie : lista_muchii)
    {
        int componenta_x = gaseste_radacina(muchie.second.first, tata);
        int componenta_y = gaseste_radacina(muchie.second.second, tata);
        if(componenta_x != componenta_y)
        {
            apm.push_back(muchie.second);
            reuniune_radacini(componenta_x, componenta_y, tata);
            cost_total += muchie.first;
        }
    }
    return apm;

}

// algoritmul lui Dijkstra implementat cu priority queue(heap de minim), va returna vectorul de distante minime
// pentru optimizare (de la 90p la 100p) verificam daca am bagat un nod deja in heap ca sa nu il avem de mai multe ori in heap
// deoarece pentru teste foarte mari este costisitor sa verificam de mai multe ori pentru acelasi nod din heap
// (accesare listei de vecini al aceluiasi nod ar fi redundanta/inutila)
vector<int> Graful_meu::dijkstra_algoritm(int nod_start)
{
    priority_queue<pereche, vector<pereche>, greater<pereche>> min_heap;
    vector<int> dist_minima(_noduri + 2, INFINIT);
    vector<bool>apartine_min_heap(_noduri + 2, false);

    min_heap.push({0, nod_start});
    dist_minima[nod_start] = 0;

    while(!min_heap.empty())
    {
        int nod_curent = min_heap.top().second;
//      int distanta_curent = min_heap.top().first;
        min_heap.pop();

        if(apartine_min_heap[nod_curent] == true)
            continue;

        apartine_min_heap[nod_curent] = true;
        for(auto vecin : _lista_adiacenta[nod_curent])
        {
            int distanta_vecin = vecin.cost;
            int nod_vecin = vecin.destinatie;

            if(dist_minima[nod_vecin] > dist_minima[nod_curent] + distanta_vecin)
            {
                dist_minima[nod_vecin] = dist_minima[nod_curent] + distanta_vecin;
                min_heap.push({dist_minima[nod_vecin], nod_vecin});
            }
        }
    }
    for(int i = 1 ; i <= _noduri ; ++i)
        if(dist_minima[i] == INFINIT)
            dist_minima[i] = 0;

    return dist_minima;
}

// algoritmul lui BellMan-Ford care ne va intoarce vectorul de distante, ne vom folosi de un priority queue(heap de minim)
// daca avem un ciclu negativ (adica daca vom avea mai mult de ((n-1) * m) iteratii, idee discutata la curs) vom afisa un mesaj corespunzator
// si vom intoarce un vector gol in acest caz
// am folosit conversia la long long, deoarece produsul ajunge in jur de 10^10
// solutia fara vectorul "ruta_mai_buna" ne ofera 85p (pica un test pe infoarena)
// acest vector de bool ne va ajuta sa nu retinem in heap de mai multe ori acelasi nod (idee aplicata la cateva probleme de mai sus)
vector<int> Graful_meu::bellmanford_algoritm(int nod_start)
{
    long long numar_iteratii = 0;
    vector<int>dist_minima(_noduri + 2, INFINIT);
    priority_queue<pereche, vector<pereche>, greater<pereche>> min_heap;
    vector<bool>ruta_mai_buna(_noduri + 2, false); // acest vector ne ajuta sa avem o complexitate mai buna

    min_heap.push({0, nod_start});
    dist_minima[nod_start] = 0;

    while(!min_heap.empty())
    {
        if(numar_iteratii > (long long) (_noduri - 1) * _muchii)      // ciclu negativ maxim aprox 12,5 * 10^9
            return vector<int>();

        int nod_curent = min_heap.top().second;
        min_heap.pop();

        ruta_mai_buna[nod_curent] = false;
        for(auto vecin : _lista_adiacenta[nod_curent])
        {
            int distanta_vecin = vecin.cost;
            int nod_vecin = vecin.destinatie;

            if(dist_minima[nod_vecin] > dist_minima[nod_curent] + distanta_vecin)
            {
                dist_minima[nod_vecin] = dist_minima[nod_curent] + distanta_vecin;
                if(ruta_mai_buna[nod_vecin] == false)
                {
                    min_heap.push({dist_minima[nod_vecin], nod_vecin});
                    ruta_mai_buna[nod_vecin] = true;
                }
            }
        }

        ++numar_iteratii;
    }

    return dist_minima;

}

// algoritmul lui Roy-Floyd returneaza matricea drumurilor (matrice_ponderi[x][y] -> drumul minim de la x la y)
vector<vector<long long>> Graful_meu::roy_floyd_algoritm(vector<vector<long long>>&mat)
{
    vector<vector<long long>>matrice_ponderi = mat;

    for(int k = 1; k <= _noduri; ++k)
        for(int i = 1; i <= _noduri; ++i)
            for(int j = 1; j <= _noduri; ++j)
                    matrice_ponderi[i][j] = min(matrice_ponderi[i][j], matrice_ponderi[i][k] + matrice_ponderi[k][j]);

    for(int i = 1; i <= _noduri; ++i)
        for(int j = 1; j <= _noduri; ++j)
            if((matrice_ponderi[i][j] == INFINIT) or (i == j))
                matrice_ponderi[i][j] = 0;

    return matrice_ponderi;
}

// aceasta metoda determina frunza cea mai indepartata de nodul 1 cu un BFS, iar apoi mai facem un BFS din cea mai indepartata frunza pentru
// a determina frunza cea mai indepartata de "noua radacina"
int Graful_meu::diametru_arbore()
{
    int diametru_maxim, index_max_el;
    vector<int> drumuri_minime_bfs_1 = bfs(1);
    index_max_el = max_element(drumuri_minime_bfs_1.begin(), drumuri_minime_bfs_1.end()) - drumuri_minime_bfs_1.begin();
    vector<int> drumuri_minime_bfs_2 = bfs(index_max_el);
    diametru_maxim = *max_element(drumuri_minime_bfs_2.begin(), drumuri_minime_bfs_2.end()) + 1;

    return diametru_maxim;
}

// acest BFS determina un lant nesaturat prin vectorul de tati si returneaza fluxul maxim prin acest lant
// metoda nu duce pana la final tot BFS-ul, ci se opreste cand a gasit un lant nesat
int Graful_meu::bfs_edmonds_karp_algoritm(int sursa, int destinatie, vector<int> &tata, vector<vector<int>>&capacitate_reziduala, vector<vector<int>>&adj)
{
    tata.clear();
    tata.resize(_noduri + 2, NIL);
    tata[sursa] = 0;
    queue<pereche>coada_nod_si_flux;
    coada_nod_si_flux.push({sursa, INFINIT});


    while(!coada_nod_si_flux.empty())
    {
        int nod_curent = coada_nod_si_flux.front().first;
        int flux = coada_nod_si_flux.front().second;
        coada_nod_si_flux.pop();

        for(auto vecin : adj[nod_curent])
        {
            if(tata[vecin] == -1 and capacitate_reziduala[nod_curent][vecin])
            {
                tata[vecin] = nod_curent;
                int flux_nou = min(flux, capacitate_reziduala[nod_curent][vecin]);
                if(vecin == destinatie)
                    return flux_nou;
                coada_nod_si_flux.push({vecin, flux_nou});
            }
        }
    }

    return 0;
}

// metoda returneaza fluxul total si , in acelasi timp, actualizeaza capacitatea reziduala a intregii retele acolo unde este necesar
int Graful_meu::flux_maxim_edmonds_karp_algoritm(int sursa, int destinatie)
{
    int flux_total = 0;
    vector<int> tata;
    int flux_nou;

    vector<vector<int>>capacitate_reziduala(_noduri + 2, vector<int>(_noduri + 2));
    vector<vector<int>>adj(_noduri + 2);

    for(int nod = 1; nod <= _noduri; ++nod)
        for(auto vecin : _lista_adiacenta[nod])
            capacitate_reziduala[nod][vecin.destinatie] = vecin.cost;

    for(int nod = 1; nod <= _noduri; ++nod)
        for(auto vecin : _lista_adiacenta[nod])
        {
            adj[nod].push_back(vecin.destinatie);
            adj[vecin.destinatie].push_back(nod);
        }

    while((flux_nou = bfs_edmonds_karp_algoritm(sursa, destinatie, tata, capacitate_reziduala, adj)))
    {
        flux_total += flux_nou;
        int nod_curent = destinatie;
        while(nod_curent != sursa)
        {
            int nod_anterior = tata[nod_curent];
            capacitate_reziduala[nod_anterior][nod_curent] -= flux_nou;
            capacitate_reziduala[nod_curent][nod_anterior] += flux_nou;
            nod_curent = nod_anterior;
        }

    }

    return flux_total;
}

// metoda cauta pentru un nod din multimea din stanga o potrivire cu un nod din multimea din dreapta care nu are asociata o potrivire
// si returneaza true daca gaseste un astfel de nod, sau false daca nu
bool Graful_meu::cauta_cuplaj_bipartit(int nod_stanga, vector<bool>&vizitat, vector<int>&potrivire)
{
    for(auto vecin : _lista_adiacenta[nod_stanga])
    {
        int nod_dreapta = vecin.destinatie;
        if(!vizitat[nod_dreapta])
        {
            vizitat[nod_dreapta] = true;
            if(potrivire[nod_dreapta] < 0 or cauta_cuplaj_bipartit(potrivire[nod_dreapta], vizitat, potrivire))
            {
                potrivire[nod_dreapta] = nod_stanga;
                return true;
            }
        }
    }

    return false;
}

// pentru fiecare nod din multimea din stanga cautam un corespondent in multimea din dreapta si cand am gasit actualizam cuplajul maxim
// vom returna vectorul cu cuplaje
vector<int> Graful_meu::cuplaj_maxim_in_graj_bipartit(int &cuplaj_maxim, int numar_noduri_stanga, int numar_noduri_dreapta)
{
    vector<int>potrivire(numar_noduri_dreapta + 2, NIL);
    cuplaj_maxim = 0;

    for(int nod_stanga = 1; nod_stanga <= numar_noduri_stanga; ++nod_stanga)
    {
        vector<bool>vizitat(numar_noduri_dreapta, false);
        if(cauta_cuplaj_bipartit(nod_stanga, vizitat, potrivire))
            ++cuplaj_maxim;
    }

    return potrivire;
}

// prin aceasta metoda facem un DFS pentru a determina muchiile din ciclul eulerian(daca avem un ciclu eulerian)
void Graful_meu::ciclu_eulerian_dfs(int nod_start, vector<bool>&vizitat, vector<int>&muchii_ciclu_eulerian)
{
    while(!_lista_adiacenta[nod_start].empty())
    {
        int nod_destinatie = _lista_adiacenta[nod_start].back().destinatie;
        int numar_ordine_muchie = _lista_adiacenta[nod_start].back().cost;
        _lista_adiacenta[nod_start].pop_back();

        if(vizitat[numar_ordine_muchie] == false)
        {
            vizitat[numar_ordine_muchie] = true;

            ciclu_eulerian_dfs(nod_destinatie, vizitat, muchii_ciclu_eulerian);
        }
    }
    muchii_ciclu_eulerian.push_back(nod_start);
}

// aici verificam daca avem un ciclu eulerian: daca un nod are grad impar atunci nu avem ciclu eulerian sau daca nu avem un graf conex
// returnam lista muchiilor care formeaza ciclul eulerian
vector<int> Graful_meu::verificare_si_constructie_ciclu_eulerian(int nod_start)
{
    vector<bool> vizitat(_muchii + 2, false);
    vector<int>muchii_ciclu_eulerian;

    for(int i = 1; i <= _noduri; ++i)
        if(_lista_adiacenta[i].size() % 2)
            return vector<int>(1, -1);

    ciclu_eulerian_dfs(nod_start, vizitat, muchii_ciclu_eulerian);

    for(int i = 1; i <= _muchii; ++i)
        if(vizitat[i] == false)
            return vector<int>(1, -1);

    muchii_ciclu_eulerian.pop_back();
    return muchii_ciclu_eulerian;
}

// returnam costul minim al unui ciclu hamiltonian folosindu-ne de metoda programarii dinamice pentru a determina pentru fiecare
// configuratie de noduri, pe care o reprezentam binar, costul minim
int Graful_meu::cost_minim_ciclu_hamiltonian()
{
    const int inf = INFINIT / 2;
    vector<vector<int>>C((1 << _noduri), vector<int>(_noduri, inf));
    int cost_total = inf;
    C[1][0] = 0;

    for(int configuratie = 1; configuratie < (1 << _noduri); ++configuratie)
        for(int i = 0; i < _noduri; ++i)
            if(configuratie & (1 << i))
                for(auto element : _lista_adiacenta[i])
                {
                    int vecin = element.destinatie;
                    int cost_vecin = element.cost;
                    if(configuratie & (1 << vecin))
                        C[configuratie][i] = min(C[configuratie][i], C[(1 << i) ^ configuratie][vecin] + cost_vecin);
                }

    for(auto nod : _lista_adiacenta[0])
        cost_total = min(cost_total, C[(1 << _noduri) - 1][nod.destinatie] + nod.cost);

    return cost_total;
}

// --------------------------------------------- FUNCTII HELPER AFISARE -------------------------------------------------

void dfs_componente_conexe_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false);
    g.citire_graf(fin);

    gout << g.componente_conexe();

    fin.close(); gout.close();
}

void bfs_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii, nod_start;
    fin >> noduri >> muchii >> nod_start;

    Graful_meu g(noduri, muchii, true);
    g.citire_graf(fin);

    vector<int>drumuri_minime = g.bfs(nod_start);
    for(int i = 1; i <= noduri; ++i)
        gout << drumuri_minime[i] << " ";

    fin.close(); gout.close();
}

void componente_biconexe_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false);
    g.citire_graf(fin);

    vector<vector<int>>toate_componentele_biconexe = g.componente_biconexe();
    gout << toate_componentele_biconexe.size() << '\n';
    for(auto &componenta_biconexa : toate_componentele_biconexe)
    {
        for(auto element : componenta_biconexa)
            gout << element << " ";
        gout << '\n';
    }

    fin.close(); gout.close();
}

void componente_tare_conexe_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true);
    g.citire_graf(fin);

    vector<vector<int>>toate_componentele_tare_conexe = g.componente_tare_conexe();
    gout << toate_componentele_tare_conexe.size() << '\n';
    for(auto &componenta_tare_conexa: toate_componentele_tare_conexe)
    {
        for(auto element : componenta_tare_conexa)
            gout << element << " ";
        gout << '\n';
    }

    fin.close(); gout.close();
}

void muchii_critice_helper()
{
    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    cin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false);
    g.citire_graf(cin);

    vector<vector<int>>toate_muchiile_critice = g.muchii_critice();
    cout << toate_muchiile_critice.size() << '\n';
    for(auto &muchie_critica : toate_muchiile_critice)
    {
        for(auto element : muchie_critica)
            cout << element << " ";
        cout << '\n';
    }
}

void sortare_topologica_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true);
    g.citire_graf(fin);

    vector<int>ordine_topologica = g.sortare_topologica();
    for(auto nod : ordine_topologica)
        gout << nod << " ";

    fin.close(); gout.close();
}

void havel_hakimi_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    int n;
    fin >> n;
    Graful_meu g(n);
    vector<int>grade(n + 2);
    for(int i = 0; i < n; ++i)
        fin >> grade[i];

    bool formare_graf = g.havel_hakimi(n, grade);
    if(formare_graf)
        gout << "Se poate forma graful!";
    else
        gout << "Nu se poate forma graful!";

    fin.close(); gout.close();
}

void prim_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii, cost_total, nod_start;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false, true);
    g.citire_graf(fin);

    nod_start = 1;
    vector<int>tata = g.prim_algoritm(cost_total, nod_start);
    gout << cost_total << '\n' << noduri - 1 << '\n';
    for(int i = 1; i <= noduri; ++i)
        if(i != nod_start)
            gout << i << " " << tata[i] << '\n';

    fin.close(); gout.close();
}

// daca ni se da operatie de tip 1 - unim arborii (stim din cerinta ca cele doua noduri nu vor fi din acelasi arbore)
//                           tip 2 - afisam "DA" daca fac parte din acelasi arbore, else "NU" daca sunt din arbori diferiti
void paduri_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    vector<int>tata(noduri + 2);
    for (int i = 1; i <= noduri; ++i)
        tata[i] = i;

    for (int i = 1; i <= muchii; ++i)
    {
        int operatie, x, y;
        fin >> operatie >> x >> y;
        int rad_de_x = Graful_meu::gaseste_radacina(y, tata);
        int rad_de_y = Graful_meu::gaseste_radacina(x, tata);
        if(operatie == 1)
            Graful_meu::reuniune_radacini(rad_de_x, rad_de_y, tata);
        else
            if(rad_de_x == rad_de_y)
                gout << "DA" << '\n';
            else
                gout << "NU" << '\n';
    }

    fin.close(); gout.close();
}

void kruskal_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii, cost_total;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false, true);
    g.citire_graf(fin);

    vector<pereche>apm = g.kruskal_algoritm(cost_total);
    gout << cost_total << '\n' << noduri - 1 << '\n';
    for(auto muchie : apm)
        gout << muchie.first << " " << muchie.second << '\n';

    fin.close(); gout.close();
}

void dijkstra_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii, nod_start;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true, true);
    g.citire_graf(fin);

    nod_start = 1;
    vector<int> distante_minime = g.dijkstra_algoritm(nod_start);
    for(int i = 1; i <= noduri; ++i)
        if(i != nod_start)
            gout << distante_minime[i] << " ";

    fin.close(); gout.close();
}

void bellman_ford_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii, nod_start;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true, true);
    g.citire_graf(fin);

    nod_start = 1;
    vector<int> distante_minime = g.bellmanford_algoritm(nod_start);
    if(distante_minime != vector<int>())
    {
        for(int i = 1; i <= noduri; ++i)
            if(i != nod_start)
                gout << distante_minime[i] << " ";
    }
    else
        gout << "Ciclu negativ!\n";

    fin.close(); gout.close();
}

void roy_floyd_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    int noduri;
    fin >> noduri;

    Graful_meu g(noduri);

    vector<vector<long long>>mat(noduri + 2, vector<long long>(noduri + 2));
    for(int i = 1; i <= noduri; ++i)
        for(int j = 1; j <= noduri; ++j)
        {
            fin >> mat[i][j];
            if(mat[i][j] == 0)
                mat[i][j] = INFINIT;
        }


    vector<vector<long long>> matrice_drumuri_minime = g.roy_floyd_algoritm(mat);
    for(int l = 1; l <= noduri; ++l)
    {
        for(int t = 1; t <= noduri; ++t)
            gout << matrice_drumuri_minime[l][t] << " ";
        gout << '\n';
    }

    fin.close(); gout.close();
}

void diametru_arbore_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    int noduri;
    fin >> noduri;

    Graful_meu g(noduri, noduri - 1);
    g.citire_graf(fin);

    gout << g.diametru_arbore();

    fin.close(); gout.close();
}

void flux_maxim_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true, true);
    g.citire_graf(fin);

    gout << g.flux_maxim_edmonds_karp_algoritm(1, noduri);

    fin.close(); gout.close();
}

void cuplaj_maxim_in_graf_bipartit_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri din multimea din stanga, apoi din dreapta, iar mai apoi numarul de muchii
    int numar_noduri_stanga, numar_noduri_dreapta, muchii, cuplaj_maxim;
    fin >> numar_noduri_stanga >> numar_noduri_dreapta >> muchii;

    Graful_meu g(numar_noduri_stanga, muchii, true, false);
    g.citire_graf(fin);

    vector<int>asociere = g.cuplaj_maxim_in_graj_bipartit(cuplaj_maxim, numar_noduri_stanga, numar_noduri_dreapta);
    gout << cuplaj_maxim << '\n';
    for(int i = 1; i <= numar_noduri_dreapta; ++i)
        if(asociere[i] != NIL)
            gout << asociere[i] << " " << i << '\n';

    fin.close(); gout.close();
}

void ciclu_eulerian_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, false, false);
    for(int i = 1; i <= muchii; ++i)
    {
        int x, y;
        fin >> x >> y;
        g.adaugare_muchie(x, y, i);
    }
    g.setter_numar_muchii(muchii);

    int nod_start = 1;
    vector<int>muchii_ciclu_eulerian = g.verificare_si_constructie_ciclu_eulerian(nod_start);
    for(auto element : muchii_ciclu_eulerian)
        gout << element << " ";

    fin.close(); gout.close();
}

void ciclu_hamiltonian_helper(string fisier_intrare, string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream gout(fisier_iesire);

    //citire numar de noduri si numar de muchii
    int noduri, muchii;
    fin >> noduri >> muchii;

    Graful_meu g(noduri, muchii, true, true);
    g.citire_graf(fin);

    int cost_total = g.cost_minim_ciclu_hamiltonian();
    if(cost_total == INFINIT / 2)
        gout << "Nu exista solutie";
    else
        gout << cost_total;

    fin.close(); gout.close();
}
int main()
{
//  dfs_componente_conexe_helper("date.in", "date.out");

//  bfs_helper("date.in", "date.out");

//  componente_biconexe_helper("date.in", "date.out");

//  componente_tare_conexe_helper("date.in", "date.out");

//  muchii_critice_helper();

//  sortare_topologica_helper("date.in", "date.out");

//  havel_hakimi_helper("date.in", "date.out");

//  prim_helper("date.in", "date.out");

//  paduri_helper("date.in", "date.out");

//  kruskal_helper("date.in", "date.out");

//  dijkstra_helper("date.in", "date.out");

//  bellman_ford_helper("date.in", "date.out");

//  roy_floyd_helper("date.in", "date.out");

//  diametru_arbore_helper("date.in", "date.out");

//  flux_maxim_helper("date.in", "date.out");

//  cuplaj_maxim_in_graf_bipartit_helper("date.in", "date.out");

//  ciclu_eulerian_helper("date.in", "date.out");

//  ciclu_hamiltonian_helper("date.in", "date.out");

    return 0;
}
