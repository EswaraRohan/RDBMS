#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseEXPORT(){
    logger.log("syntacticParseEXPORT");

    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORT;
    parsedQuery.exportRelationName = tokenizedQuery[1];
    return true;
}

bool syntacticParseMATEXPORT(){
    logger.log("syntacticParseMATEXPORT");

        if(tokenizedQuery.size()!=3){
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = MATEXPORT;
        parsedQuery.exportMatrixName = tokenizedQuery[2];
        return true;

}

bool semanticParseMATEXPORT()
{
    logger.log("semanticParseMATEXPORT");
    if (matrixCatalogue.isMatrix(parsedQuery.exportMatrixName))
        return true;
    cout << "SEMANTIC ERROR: No such matrix exists" << endl;
    return false;
}

bool semanticParseEXPORT()
{
    logger.log("semanticParseEXPORT");
    //Table should exist
    if (tableCatalogue.isTable(parsedQuery.exportRelationName))
        return true;
    cout << "SEMANTIC ERROR: No such relation exists" << endl;
    return false;
}

void executeMATEXPORT(){
    logger.log("executeMATEXPORT");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportMatrixName);
    matrix->makePermanent(parsedQuery.exportMatrixName);
    return;
}

void executeEXPORT()
{
    logger.log("executeEXPORT");
    Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
    table->makePermanent();
    return;
}