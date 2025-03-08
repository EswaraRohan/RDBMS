#include "global.h"

bool MatrixCatalogue::isMatrix(string matrixName)
{
    logger.log("MatrixCatalogue::isMatrix"); 
    if (this->matrices.count(matrixName))
        return true;
    return false;
}

void MatrixCatalogue::addMatrix(Matrix* matrix){
    logger.log("matrixCatalogue::addMatrix");
    this->matrices[matrix->MatrixName] = matrix;
    return; 
}

void MatrixCatalogue::delMatrix(string matrixName){
    logger.log("matrixCatalogue::delMatrix)");
    this->matrices[matrixName]->unload();
    delete this->matrices[matrixName];
    this->matrices.erase(matrixName);
}

Matrix* MatrixCatalogue::getMatrix(string matrixName){
    logger.log("matrixCatalogue::getMatrix");
    Matrix* matrix = this->matrices[matrixName];
    return matrix;
}

void MatrixCatalogue::print(){
    logger.log("matrixCatalogue::print");
    cout<< "\n Matrice\n";

    int rowCnt = 0;
    for(auto matrix: this->matrices)
    {
        cout<<matrix.first<<endl;
        rowCnt++;
    }
    printRowCount(rowCnt);
}

MatrixCatalogue::~MatrixCatalogue(){
    logger.log("matrixCatalogue::~matrixCatalogue");
    for(auto matEntry: this->matrices){
        matEntry.second->unload();
        delete matEntry.second;
    }
}

