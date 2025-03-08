#include <global.h>

bool syntacticParseCTRANSPOSE(){
    logger.log("syntacticParseCTRANSPOSE");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        cout << "Error in syntactic" << endl;
        return false;
    }
    parsedQuery.queryType = CTRANSPOSE;
    parsedQuery.ctransposeMatrixname1 = tokenizedQuery[1];
    parsedQuery.ctransposeMatrixname2 = tokenizedQuery[2];
    return true;
}

bool semanticParseCTRANSPOSE(){
    logger.log("semanticParseCTRANSPOSE");
    if (!((matrixCatalogue.isMatrix(parsedQuery.ctransposeMatrixname1)) && (matrixCatalogue.isMatrix(parsedQuery.ctransposeMatrixname2))))    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeCTRANSPOSE()
{
    logger.log("executeCROSS_TRANSPOSE");

    if(tokenizedQuery[1] == tokenizedQuery[2]){
        Matrix* matrix2 = matrixCatalogue.getMatrix(parsedQuery.ctransposeMatrixname2);
        matrix2->CROSS_TRANSPOSE();
    }

    else{
        Matrix* matrix1 = matrixCatalogue.getMatrix(parsedQuery.ctransposeMatrixname1);
        matrix1->CROSS_TRANSPOSE();
        string s1 = matrix1->matrixname();
        Matrix* matrix2 = matrixCatalogue.getMatrix(parsedQuery.ctransposeMatrixname2);
        matrix2->CROSS_TRANSPOSE();
        string s2 = matrix2->matrixname();
        matrix1->makePermanenthere(s2);
        matrix2->makePermanenthere(s1);
    
    }

    return;
}