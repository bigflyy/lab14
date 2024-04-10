// Lab14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
// make adj matrix from edges
const char* FILE_ADJ_IMPORT = "adjMatrixIn.txt";
const char* FILE_ADJ_EXPORT = "adjMatrixOut.txt";
// make LSD from matrix
const char* FILE_NUM_IMPORT = FILE_ADJ_EXPORT;
const char* FILE_NUM_EXPORT = "numOut.txt";
// make edges from LSD
const char* FILE_EDGES_OUTPUT = "edgesOut.txt";
int q{ 0 };
int waysToL{ 0 };


void outputMatrixAdj(int n, int m, int**& M, const char* fileName)
// n - кол-во вершин
// m - кол-во ребер
{
    FILE* file;
    int i, j;

    // открываем файл 
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    // вписываем n и m
    fprintf(file, "%d %d\n", n, m);

    // вписываем коэффиценты
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fprintf(file, "%d ", M[i][j]);
        }
        fprintf(file, "\n");
    }

    // 
    fclose(file);

    //printf("Matrix has been successfully written to the file.\n");
}

//void outputMatrixVector(int n, int k, std::vector<std::vector<int>>& M, const char* fileName)
//// первая строка файла - количество уравнений (строк)   n
//// вторая строка - количество неизвестных (столбцов-1)    k
//// После этого следуют n строк коэффициентов и свободных членов уравнений.
//// В каждой строке  k коэффициентов и свободный член определенного уравнения.
//{
//    FILE* file;
//    int i, j;
//
//    // открываем файл 
//    file = fopen(fileName, "w");
//    if (file == NULL)
//    {
//        printf("Error opening the file.\n");
//        return;
//    }
//
//    // вписываем n и k
//    fprintf(file, "%d %d\n", n, k);
//
//    // вписываем коэффиценты
//    for (i = 0; i < n; i++)
//    {
//        for (j = 0; j < k + 1; j++)
//        {
//            fprintf(file, "%d ", M[i][j]);
//        }
//        fprintf(file, "\n");
//    }
//
//    // 
//    fclose(file);
//
//    printf("Matrix has been successfully written to the file.\n");
//}

void importMatrixAdj(int& n, int& m, int**& M, const char* fileName, bool minusOneEnd=false, bool isOriented = false)
// первая строка файла - количество уравнений (строк)   n
// вторая строка - количество неизвестных (столбцов -1, т.к. без свободных чл.)    k
// После этого следуют n строк коэффициентов и свободных членов уравнений.
// В каждой строке  k коэффициентов и свободный член определенного уравнения.
{
    int i{ 0 };
    int j{ 0 };
    FILE* file;
    // Открываем файл
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    // читаем n - количество вершин (0,1,2,...,n-1)
    fscanf(file, "%d", &n);
    M = new int* [n]; // строки [][][]
    for (int i = 0; i < n; i++)
    {
        M[i] = new int[n];
        for (int j = 0; j < n; j++) M[i][j] = 0; // столбцы, везде ноль. 
    }
    if (!minusOneEnd)
    {
        fscanf(file, "%d", &m); // m - количество ребер. 
        for (int k = 0; k < m; k++)
        {
            fscanf(file, "%d %d", &i, &j); // i j is understandable. 
            M[i][j] = 1;
            if (!isOriented)
            {
                M[j][i] = 1;
            }

        }
    }
    else
    {
        m = 0;
        int first{};
        int second{};
        while (i != -1)
        {
            fscanf(file, "%d %d", &i, &j);
            if (i != -1 and j != -1)
            {
                M[i][j] = 1;
                if (!isOriented)
                {
                    M[j][i] = 1;
                }
                m++;
            }

        }
    }


    fclose(file);
}
int** adjMatrixToEdges(int& n, int& m, const char* fileName, bool isOriented=false)
{
    int** edgesList;
    FILE* file;
    // Открываем файл
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return nullptr;
    }
    fscanf(file, "%d%d", &n, &m);
    //if (!isOriented)
    //{
    //    m *= 2; // в два раза больше ребер 
    //}
    edgesList = new int* [m]; // строки [][][] столько сколько ребер
    for (int i = 0; i < m; i++)
    {
        edgesList[i] = new int[2]; // каждое ребро - 2 вершины
        // каждое отдельное ребро 
        for (int j = 0; j < 2; j++)
        {
            edgesList[i][j] = -1; // how and why is this -1 // okay so it isnt 0,0. but it leaves some of them being -1. 
        }
    }
    int isAdj{ 0 };
    int edgesListCounter{ 0 };
    // чтение из файла But you can only read non oriented graphs like this otherwise you lose data
    if (!isOriented)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                fscanf(file, "%d", &isAdj);
                // skip first part чтобы без повторяющихся. but still skip numbers
                if (j < i) { continue; }
                else if (isAdj == 0) { continue; }
                else if (isAdj == 1)
                {
                    edgesList[edgesListCounter][0] = i; // 5-ое ребро пытается добавиться почему ребер больше чем в фалйе 
                    edgesList[edgesListCounter][1] = j;
                    edgesListCounter++;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                fscanf(file, "%d", &isAdj);
                if (isAdj == 0) { continue; }
                else if (isAdj == 1)
                {
                    edgesList[edgesListCounter][0] = i; // 5-ое ребро пытается добавиться почему ребер больше чем в фалйе 
                    edgesList[edgesListCounter][1] = j;
                    edgesListCounter++;
                }
            }
        }
    }
    fclose(file);
    return edgesList;
}

void outputMatrixNum(const int n, const int m, int*& D, int*& S, int*& L, const char* fileName, bool isFileForReading = false, bool isFirstTask = true, bool isOriented=false)
// R stands for readable
{
    FILE* file;
    int i, j;
    if (isFileForReading && isFirstTask)
    {
        fileName = "1-numOutRead.txt";
    }
    // открываем файл 
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    // вписываем n и m
    fprintf(file, "%d %d\n", n, m);
    //    D = new int[m + m]; S = new int[n];
    //    L = new int[n]; U = new int[n];
    
    if(isFileForReading) fprintf(file, "L: ");
    for (int i = 0; i < n; i++)
    {
        fprintf(file, "%d ", L[i]);
    }
    fprintf(file, "\n");

    if (isFileForReading) fprintf(file, "S: ");
    for (int i = 0; i < n; i++)
    {
        fprintf(file, "%d ", S[i]);
    }
    fprintf(file, "\n");

    if (isFileForReading) fprintf(file, "D: ");

    int sizeOfD{ 0 };
    if (!isOriented) sizeOfD = m + m;
    else sizeOfD = m;

    for (int i = 0; i < sizeOfD; i++)
    {
        fprintf(file, "%d ", D[i]);
    }
    fprintf(file, "\n");

    fclose(file);

    //printf("Matrix has been successfully written to the file.\n");
}
void importMatrixLSD(int& n, int& m, int*& D, int*& S, int*& L, const char* fileName)
{
    int k{};
    int* v1, * v2, * U;
    int i{ 0 };
    int j{ 0 };
    FILE* file;
    // Открываем файл
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fscanf(file, "%d%d", &n, &m);

    D = new int[m + m]; S = new int[n];
    L = new int[n]; U = new int[n];
    // обнуление длин списков
    for (j = 0; j < n; j++) L[j] = 0;
    int el{};
    for (j = 0; j < n; j++)
    {
        fscanf(file, "%d", &el);
        L[j] = el;
    }
    for (j = 0; j < n; j++)
    {
        fscanf(file, "%d", &el);
        S[j] = el;
    }
    for (j = 0; j < (m + m); j++)
    {
        fscanf(file, "%d", &el);
        D[j] = el;
    }
    fclose(file);

}

void edgesToLSD(int& n, int& m, int*& D, int*& S, int*& L, const char* fileName, bool isMinusOneEnd)
{
    int k{};
    int* v1, * v2, * U;
    int i{ 0 };
    int j{ 0 };
    FILE* file;
    // Открываем файл
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }
    fscanf(file, "%d", &n);
    int** edge = new int* [n * n]; // maximum amount of edges
    if (!isMinusOneEnd)
    {
        fscanf(file, "%d", &m);
        int** edge = new int* [m];
        for (int i = 0; i < m; i++)
        {
            edge[i] = new int[2];
        }
        for (int i = 0; i < m; i++)
        {
            fscanf(file, "%d %d", &edge[i][0], &edge[i][1]);
        }
    }
    else
    {
        for (int i = 0; i < n*n; i++)
        {
            edge[i] = new int[2];
        }
        m = 0;
        int first{};
        int second{};
        while (first != -1)
        {
            fscanf(file, "%d %d", &first, &second);
            if (first != -1 and second != -1)
            {
                edge[m][0] = first; edge[m][1] = second;
                m++;
            }

        }
    }

    //scanf("%d%d", &n, &m);
    v1 = new int[m]; v2 = new int[m];
    for (i = 0; i < m; i++)
    {
        v1[i] = edge[i][0];
        v2[i] = edge[i][1];
    }

    D = new int[m + m]; S = new int[n];
    L = new int[n]; U = new int[n];
    // обнуление длин списков
    for (j = 0; j < n; j++) L[j] = 0;
    // вычисление длин списков
    for (i = 0; i < m; i++)
    {
        L[v1[i]]++; L[v2[i]]++;
    }
    S[0] = 0; // Вычисление начальных индексов на списки в массиве DD
    for (j = 1; j < n; j++) S[j] = S[j - 1] + L[j - 1];
    for (j = 0; j < n; j++) U[j] = S[j];
    // дублирование начальных индексов
    for (i = 0; i < m; i++) // распределение смежных вершин
        // по спискам массива D
    {
        k = v1[i]; D[U[k]] = v2[i]; U[k]++;

        k = v2[i]; D[U[k]] = v1[i]; U[k]++;
    }
    fclose(file);
}
// конвертировать матрицу смежности в плотный список смежных вершин. 
// doesnt even remotely work 
void importMatrixNum(int& n, int& m, int*& D, int*& S, int*& L, const char* fileName, bool isOriented=false)
// номер списка = номер вершины. эта вершина с чем смежна - содержание списка. 
{
    int k{};
    int* v1, * v2, * U;
    int i{ 0 };
    int j{ 0 };
    FILE* file;

    auto edge = adjMatrixToEdges(n, m, fileName, isOriented); 

    if (!isOriented)
    {
        v1 = new int[m]; v2 = new int[m];
        for (i = 0; i < m; i++)
        {
            v1[i] = edge[i][0];
            v2[i] = edge[i][1];
        }

        D = new int[m + m]; S = new int[n];
        L = new int[n]; U = new int[n];
        // обнуление длин списков
        for (j = 0; j < n; j++) L[j] = 0;
        // вычисление длин списков
        for (i = 0; i < m; i++)
        {
            L[v1[i]]++; L[v2[i]]++;
        }
        S[0] = 0; // Вычисление начальных индексов на списки в массиве DD
        for (j = 1; j < n; j++) S[j] = S[j - 1] + L[j - 1];
        for (j = 0; j < n; j++) U[j] = S[j];
        // дублирование начальных индексов
        for (i = 0; i < m; i++) // распределение смежных вершин
            // по спискам массива D
        {
            // it is v1 and v2 because if it is not oriented then it is adjacent to bth
            k = v1[i]; D[U[k]] = v2[i]; U[k]++; // it is kinda place where to put next k element in D. 
            // and becaues it is not oriented we need to put v2 for v1 and v1 for v2. but if it isnt!!! we dont. cuz if we have v1-v2 it doesnt mean we have v2-v1
            k = v2[i]; D[U[k]] = v1[i]; U[k]++;
        }
    }
    else
    { // TODO: rewrite for oriented
        v1 = new int[m]; v2 = new int[m];
        for (i = 0; i < m; i++)
        {   
            v1[i] = edge[i][0];
            v2[i] = edge[i][1];
        }

        D = new int[m]; // m instead of m+m bcz oriented
        S = new int[n];
        L = new int[n]; U = new int[n];
        // обнуление длин списков
        for (j = 0; j < n; j++) L[j] = 0;
        // вычисление длин списков
        for (i = 0; i < m; i++)
        {
            L[v1[i]]++; 
            // only v1 for L bcz oriented
        }
        S[0] = 0; // Вычисление начальных индексов на списки в массиве DD
        for (j = 1; j < n; j++) S[j] = S[j - 1] + L[j - 1];
        for (j = 0; j < n; j++) U[j] = S[j];
        // дублирование начальных индексов
        for (i = 0; i < m; i++) // распределение смежных вершин
            // по спискам массива D
        {
            k = v1[i]; D[U[k]] = v2[i]; U[k]++; // it is kinda place where to put next k element in D. 
        }
    }
    
}


void outputEdges(int n, int m, int** edgesList, const char* fileName)
{
    FILE* file;
    // Открываем файл
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }
    fprintf(file, "%d %d", n, m);
    fprintf(file, "\n");
    for (int i = 0; i < m; i++)
    {
        fprintf(file, "%d %d", edgesList[i][0], edgesList[i][1]);
        fprintf(file, "\n");
    }
    fclose(file);
}
void convertLSD(int n, int m, int*& D1, int*& S1, int*& L1)
{
    int doubleM = m + m;
    int** edgesList = new int* [doubleM]; // m - столько ребер
    for (int i = 0; i < doubleM; i++)
    {
        edgesList[i] = new int[2]; // каждое ребро - 2 вершины
        // каждое отдельное ребро 
        for (int j = 0; j < 2; j++)
        {
            edgesList[i][j] = -1;
        }
    }


    int edgesListCount{ 0 };
    // we got lsd
    // i - номер списка (номер 1-ой вершины)
    for (int i = 0; i < n; i++)
    {
        // номер первого элемента В списке - S[i]
        for (int k = 0; k < L1[i]; k++)
        {
            edgesList[edgesListCount][0] = i; // same problem prints same stuff twice. bcz 13 and 31
            // 0 элемент в списке, 1 элемент в списке, ... пока до длина не дойдем.
            edgesList[edgesListCount][1] = D1[S1[i] + k];
            edgesListCount++;
        }
    }
    // make it import these edges and into adj matrix and into edges again kek. adjMatrixToEdges() and hide it all into a function
    outputEdges(n, doubleM, edgesList, FILE_EDGES_OUTPUT);
    //int** M;
    //importMatrixAdj(n, doubleM, M, FILE_EDGES_OUTPUT);
    //outputMatrixAdj(n, m+1, M, "a.txt");
    //auto edges = adjMatrixToEdges(n, m, "a.txt"); //  has one less element and should i even bother making it half? ?? ? 
    //outputEdges(n, m, edges, FILE_EDGES_OUTPUT);
}
void deep(int k, int n, int**& M, int* R, int nom)
{
    int i;
    // просмотр по матрице все вершины которые смежны с данной. 
    for (i = 0; i < n; i++)
    {
        
        if ((M[k][i] == 1) && (R[i] == 0)) // if adjacent and не просмотренны, 
        {
            nom++; // nom - номер вершины с которой начинается просмотр.  
            R[i] = nom; // массив R содержит номера вершин в порядке просмотра  nom isnt vertex. it is каким он попал в этот путь. i-й vertex. 
            deep(i, n, M, R, nom);
        }
    }
}

void deep3(int k, int n, int**& M, int* R, int nom, int target_vertex, bool visited[])
{
    visited[k] = true; // Mark the current vertex as visited

    // If the current vertex is the target vertex, print the path
    if (k == target_vertex)
    {
        // here suddenly is different R... and the values are different from what they should be. 
        waysToL++;
        printf("Путь: ");
        for (int i = 0; i < nom+1; i++) // it doesnt always have n  of them... actualy only nom+1 of them ? 
        {
            if (R[i] != -1)
            {
                printf("%d ", R[i]);  // 0 вошел R[0], 1 вершина вошла R[1]-ой ... нужно вывести вершины в порядке их. 
            }

        }
        printf("\n");
    }
    else
    {
        // Recursive DFS traversal to explore all neighbors of the current vertex
        for (int i = 0; i < n; i++) // if k - первая а не рекурсивная => nom = 0
        {
            if (M[k][i] == 1 && !visited[i]) // SOMEHOW k = 0 on second kind of iteration which was supposed to be starting from 0.
            {
                 // Increment the vertex number it gets incremented one more than it should. like it is still number 1 element i mean second but is 2 because .... ..
                R[nom+1] = i; // nom is somehow 1 instead of 0 . o because first iteration completes and it gets num++ and second iteration even tho it is iteration for fist recursion still has already on num higher. 
                deep3(i, n, M, R, nom+1, target_vertex, visited);
            } // 0 2 3 (supposed to be bcz R[1] = 2) 
        }
    }

    visited[k] = false; // Mark the current vertex as unvisited (backtrack)
}
void cdeep(int k, int*& L, int*& S, int*& D, int*& C)
{
    int i, j;
    // will it work for looking for L? i dont think so. 
    for (i = S[k]; i < S[k] + L[k]; i++)
    {
        j = D[i];
        if (C[j] == 0) { C[j] = q; cdeep(j, L, S, D, C); }
    }
}
void output2(int n, int*& C, const char* fileName)
// n - кол-во вершин
// m - кол-во ребер
{
    FILE* file;
    int i, j;

    // открываем файл 
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "%d семей\n", q);
    for (int i = 1; i <= q; i++)
    {
        fprintf(file, "%d семья: " , i);
        for (int j = 0; j < n; j++)
        {
            if (C[j] == i)
            {
                fprintf(file, "%d ", j);// j-я вершина входит в i-ую компоненту связности 
            }
        }
        fprintf(file, "\n");
    }

    // 
    fclose(file);

    //printf("Matrix has been successfully written to the file.\n");
}

int main()
{
    //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 1.1 ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    int** M, i, j, n, m, k;
    importMatrixAdj(n, m, M, FILE_ADJ_IMPORT);
    outputMatrixAdj(n, m, M, FILE_ADJ_EXPORT);

    // Освобождаем память
    for (i = 0; i < n; i++)
        delete[] M[i];
    delete[] M;

    //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 1.2 ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    //int* v1, * v2, * D, * S, * L, * U;
    int *D0, *S0, *L0;
    n = m = 0;

    importMatrixNum(n, m, D0, S0, L0, FILE_NUM_IMPORT);
    outputMatrixNum(n, m, D0, S0, L0, FILE_NUM_EXPORT);
    outputMatrixNum(n, m, D0, S0, L0, FILE_NUM_EXPORT,true);

    // Освобождаем память
    delete[] L0;
    delete[] S0;
    delete[] D0;

    //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 1.3 ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    n = m = 0;
    int* D1, * S1, * L1;
    importMatrixLSD(n, m, D1, S1, L1, FILE_NUM_EXPORT);
    convertLSD(n, m, D1, S1, L1);

    delete[] L1;
    delete[] S1;
    delete[] D1;

    ////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 2 ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    n = m = 0;
    int* D2, * S2, * L2;
    int** M2;
    // adj matrix
    importMatrixAdj(n, m, M, "2.txt", true);
    outputMatrixAdj(n, m, M, "2adj.txt");

    edgesToLSD(n, m, D2,S2,L2,"2.txt", true); // could just convert importMatrixNum in the end (convert adj to lsd) 
    int* C = new int[n];
    for (i = 0; i < n; i++) C[i] = 0;
    q = 0;
    for (i = 0; i < n; i++)
    {
        if (C[i] == 0)
        {
            q++; C[i] = q;
            cdeep(i, L2, S2, D2, C);
        }
    }
    // вывод в консоль
    std::cout << "C : ";
    for (int i = 0; i < n; i++)
    {
        std::cout << C[i] << ' ';
    }
    std::cout << "\n";
    std::cout << q << " семей\n";
    for (int i = 1; i <= q; i++)
    {
        std::cout << i << " семья: ";
        for (int j = 0; j < n; j++)
        {
            if (C[j] == i)
            {
                std::cout << j << " "; // j-я вершина входит в i-ую компоненту связности 
            }
        }
        std::cout << "\n";
    }
    // вывод в файл
    output2(n, C, "2out.txt");
    outputMatrixNum(n, m, D2, S2, L2, "2spisok.txt", true);
    delete[] D2;
    delete[] S2;
    delete[] L2;

    ////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 3 ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    int* L3, *D3, *S3;
    n = m = 0;
    int** M3;

    importMatrixAdj(n, m, M3, "3.txt", false, true);

    // do we need spisok?
    // well we can do deep3 to use spisok and just for that. cuz task say smh about score. 
    int* R = new int[n];
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }
    for (i = 0; i < n; i++) R[i] = -1; // -1 = не расмотрен
    R[0] = 0; // первый элемент добавленный в путь это 0
    int nom = 0; // нулевой элемент массива. 
    deep3(0, n, M3, R, nom, 9, visited);
    std::cout << "Путей в Л: " << waysToL << "\n";
    outputMatrixAdj(n, m, M3, "3adj.txt");
    importMatrixNum(n, m, D3, S3, L3, "3adj.txt", true);
    outputMatrixNum(n, m, D3, S3, L3, "3spisok.txt", true, false, true);
    delete[] L3;
    delete[] D3;
    delete[] S3;
    for (int i = 0; i < n; i++)
    {
        delete[] M3[i];
    }
    delete[] M3;

    //////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 4 //////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    n = m = i = j = 0;
    const int MAX_LAB_SIZE = 20;
    const int MAX_L_SIZE = MAX_LAB_SIZE + 2;
    const int MAX_S_SIZE = MAX_LAB_SIZE + 1;
    const int WALL_NUM = 1000;
    const int MAX_M_SIZE = (MAX_LAB_SIZE * MAX_LAB_SIZE) + 1; // кол-во выполений цикла не может превышать кол-во клеток.
    int L[MAX_L_SIZE][MAX_L_SIZE], i0, j0, ik, jk;
    char S[MAX_S_SIZE];

    const char* fileName = "4.txt";
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
    }

    // ввод данных, формирование структуры лабиринта
    fscanf(file, "%d\n", &n);
    for (i = 1; i <= n; i++)
    {
        fgets(S, MAX_LAB_SIZE + 1, file); // s - строка, нужно получать строки построчно. (row)
        for (j = 1; j <= n; j++)
        {
            if (S[j - 1] == '-')
            {
                L[i][j] = 0;
            }
            else if (S[j - 1] == 's')
            {
                L[i][j] = 0;
                i0 = i;
                j0 = j;
            }
            else if (S[j - 1] == 'f')
            {
                L[i][j] = 0;
                ik = i; // or should it be -1 ????? 
                jk = j;
            }
            else L[i][j] = WALL_NUM;
        }

    }
    for (j = 0; j <= n + 1; j++)
    {
        L[0][j] = WALL_NUM; L[n + 1][j] = WALL_NUM;
    }
    for (i = 1; i <= n; i++)
    {
        L[i][0] = WALL_NUM; L[i][n + 1] = WALL_NUM;
    }
    //fscanf(file, "%d%d\n", &i0, &j0); REPLACED WITH S AND F
    //fscanf(file, "%d%d\n", &ik, &jk);

    // Разметка лабиринта
    i = j = 0;
    int Pi[MAX_M_SIZE], Pj[MAX_M_SIZE], r, t;
    Pi[1] = i0; Pj[1] = j0; r = 1; t = 1;
    L[i0][j0] = 1;
    while (t <= r)
    {
        i = Pi[t]; j = Pj[t]; q = L[i][j] + 1;
        if (L[i - 1][j] == 0)
        {
            L[i - 1][j] = q; r++; Pi[r] = i - 1; Pj[r] = j;
        }
        if (L[i][j - 1] == 0)
        {
            L[i][j - 1] = q; r++; Pi[r] = i; Pj[r] = j - 1;
        }
        if (L[i + 1][j] == 0)
        {
            L[i + 1][j] = q; r++; Pi[r] = i + 1; Pj[r] = j;
        }
        if (L[i][j + 1] == 0)
        {
            L[i][j + 1] = q; r++; Pi[r] = i; Pj[r] = j + 1;
        }
        t++;
    }

    // Отслеживание кратчайшего пути по разметке
    i = j = k = 0;
    int Mi[MAX_M_SIZE], Mj[MAX_M_SIZE];
    k = L[ik][jk]; i = ik; j = jk;
    while (k > 0)
    {
        Mi[k] = i; Mj[k] = j;
        if (L[i - 1][j] < L[i][j]) i--;
        else if (L[i][j - 1] < L[i][j]) j--;
        else if (L[i + 1][j] < L[i][j]) i++;
        else j++;
        k--;
    }



    /*const char** LChar = new char*[MAX_LAB_SIZE];*/
    //for (int i = 0; i < MAX_LAB_SIZE; i++)
    //{
    //    LChar[i] = new char[MAX_LAB_SIZE];
    //}

    fclose(file);

    FILE* file1 = fopen("4.txt", "r");
    fscanf(file1, "%d\n", &n);
    std::vector<std::string> rows{ };

    for (i = 0; i < n; i++)
    {
        fgets(S, MAX_LAB_SIZE + 1, file1); // s - строка, нужно получать строки построчно. (row) we==
        rows.push_back(S); // UNDONE why didnt LChar[i][..] work? 
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << rows[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n\n";

    // io jo - start ik jk - end 

    if (L[ik][jk] != 0)
    {

        for (int i = 1; i <= L[ik][jk]; i++)
        {
            if (rows[Mi[i] - 1][Mj[i] - 1] != 's' && rows[Mi[i] - 1][Mj[i] - 1] != 'f')
            {
                rows[Mi[i] - 1][Mj[i] - 1] = '*';
            }// Mi[i] 0th isnot assigned"
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << rows[i][j];
            }
            std::cout << "\n";
        }

    }
    else
    {
        std::cout << "Из s невозможно попасть в t.";
    }
    //for (int i = 0; i < MAX_LAB_SIZE; i++)
    //{
    //    delete[] LChar[i];
    //}
    //delete[] LChar;

    fclose(file1);
    fclose(file);
    //////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// 5 //////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    int n1, m1, n2, m2, n3, m3;
    n1 = m1 = m2 = n2 = n3 = m3 = 0;
    int* D51, * L51, * S51, *L52, *S52, *D52, *L53, *S53, *D53;
    int** M51;
    int** M52;
    int** M53;
    importMatrixAdj(n1, m1, M51, "5-1.txt");
    importMatrixAdj(n2, m2, M52, "5-2.txt");
    importMatrixAdj(n3, m3, M53, "5-3.txt");
    outputMatrixAdj(n1, m1, M51, "5-1adj.txt");
    outputMatrixAdj(n2, m2, M52, "5-2adj.txt");
    outputMatrixAdj(n3, m3, M53, "5-3adj.txt");

    importMatrixNum(n1, m1, D51, S51, L51, "5-1adj.txt");
    importMatrixNum(n2, m2, D52, S52, L52, "5-2adj.txt");
    importMatrixNum(n3, m3, D53, S53, L53, "5-3adj.txt");
    outputMatrixNum(n1, m1, D51, S51, L51, "5-1spisok.txt");
    outputMatrixNum(n2, m2, D52, S52, L52, "5-2spisok.txt");
    outputMatrixNum(n3, m3, D53, S53, L53, "5-3spisok.txt");
    int ib = 0;
    ib = ik = 0;
    q = 0;
    int graphNum{ 1 };
    for (auto L_ : {L51, L52, L53})
    {
        for (i = 0; (i < n) && (q < 3); i++)
        {
            if (L_[i] & 1) // считаем кол-во вершин, которым инцидентно нечетное количество ребер
            {
                q++; ib = ik; ik = i;
            }

        }
        // так как эйлеров цикл существует когда все вершины графа инцидентны четному числу рёбер
        if (q == 0) // существует эйлеров цикл
        {
            
            std::cout << "Фигуру " << graphNum << " можно нарисовать, начиная и заканчивая рисование в одной и той же точке.\n";
        }
        // эйлеров путь существует когда две вершины инцидентны нечетному количеству ребер, а остальные четному. 
        else if (q == 2) // существует эйлеров путь
        {
            std::cout << "Фигуру " << graphNum << " можно нарисовать, начиная и заканчивая рисование в различных точках.\n";
        }
        else // не существует
        {
            std::cout << "Фигуру " << graphNum << " нельзя нарисовать эту фигуру (без наложения линий), не отрывая карандаша от бумаги.\n";
        }
        graphNum++;
    }


}


////////////// эйлеров цикл по матр смежности M  стр 85
//p = new struct el;
//pb = p; p->s = 0; p->p = NULL;
//for (i = 0; i < n) U[i] = 0;
//while (p != NULL)
//{
//    j = p->s; k = U[j];
//    while ((k < n) && (M[j][k] == 0)) k++;
//    U[j] = k;
//    if (k < n)
//    {
//        p1 = p->p; i0 = p->s; i = -1; p0 = p;
//        while (i != i0)
//        {
//            p2 = new struct el;
//            p->p = p2; i = p->s; k = U[i];
//            while (M[i][k] == 0) k++;
//            U[i] = k; M[i][k] = 0;
//            M[k][i] = 0; //
//            i = k; p2->s = k; p = p2;
//        }
//        p->p = p1; p = p0;
//    }
//    else p = p->p;
//}


/////////////////////////////////// Эйлеров путь на неор. графе стр89 
//for (i = 0; i < n) U[i] = 0;
//p = new struct el; pb = p; p->s = ib; i = ib;
//k = 0;
//while (M[i][k] == 0) k++;
//while (k < n)
//{
//    p1 = new struct el; p->p = p1;
//    M[i][k] = 0;
//    M[k][i] = 0; // -
//    k++;
//    while (k < n && M[i][k] == 0) k++;
//    U[i] = k;
//    p1->s = k; p = p1; i = k;
//}
//p->p = NULL;
// Аналогично программе из примера 15.6 будет построен
// линейный список, в первом элементе котрого вершина ib
// а последнем ik. для построения всего эйлерова пути после этой программы небоходимо выполнить программу из примера 15.4 удалив в ней начальные присваивания перед главным циклом.
// пример 15.4:
//while (p != NULL)
//{
//    j = p->s; k = U[j];
//    while ((k < n) && (M[j][k] == 0)) k++;
//    U[j] = k;
//    if (k < n)
//    {
//        p1 = p->p; i0 = p->s; i = -1; p0 = p;
//        while (i != i0)
//        {
//            p2 = new struct el;
//            p->p = p2; i = p->s; k = U[i];
//            while (M[i][k] == 0) k++;
//            U[i] = k; M[i][k] = 0;
//            M[k][i] = 0; //
//            i = k; p2->s = k; p = p2;
//        }
//        p->p = p1; p = p0;
//    }
//    else p = p->p;
//}