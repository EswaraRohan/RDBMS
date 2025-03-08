#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD relation_name
 */
bool syntacticParseLOAD()
{
    logger.log("syntacticParseLOAD");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LOAD;
    parsedQuery.loadRelationName = tokenizedQuery[1];
    return true;
}
bool syntacticParseMATLOAD(){
    logger.log("syntacticParseMATLOAD");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = MATLOAD;
    parsedQuery.loadMatrixName = tokenizedQuery[2];
    return true;

}
bool semanticParseMATLOAD(){
    logger.log("semanticParseMATLOAD");
    if (matrixCatalogue.isMatrix(parsedQuery.loadMatrixName)){
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}


bool semanticParseLOAD()
{
    logger.log("semanticParseLOAD");
    if (tableCatalogue.isTable(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Relation already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeMATLOAD()
{
    
    logger.log("executeMATLOAD");

    Matrix *matrix = new Matrix(parsedQuery.loadMatrixName);
    if (matrix->load()){
        matrixCatalogue.addMatrix(matrix);
        cout << "Loaded Matrix. Column Count: " << matrix->matrixdimension << " Row Count: " << matrix->rowsnum << endl;
    }
    else{
        cout<<"Load Unsuccessful"<<endl;
    }
    return;
}

void executeLOAD()
{
    logger.log("executeLOAD");

    Table *table = new Table(parsedQuery.loadRelationName);
    if (table->load())
    {
        tableCatalogue.insertTable(table);
        cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    }
    return;
}