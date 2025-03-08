#include "matrix.h"

class MatrixCatalogue
{
    unordered_map<string, Matrix*> matrices;

    public:
    MatrixCatalogue() {}
    bool isMatrix(string matName);
    void addMatrix(Matrix* matrix);
    void delMatrix(string matName);
    Matrix* getMatrix(string matName);
    void print();
    ~MatrixCatalogue();
};