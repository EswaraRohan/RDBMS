#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);

    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}



Page::Page(string entity,int pageIndex,vector<int> elements,int elecount){
    this->pType = 1;
    logger.log("Page::Page");
    this->entity = entity;
    this->pageIndex = pageIndex;    
    this->matrixelems = elements;
    this->elecount = elecount;
    this->pageName = "../data/temp/"+this->entity + "_Page" + to_string(pageIndex);
}


void Page::writeMatrixPage(){
    logger.log("Page::writeMatrixPage");
    ofstream fout(this->pageName, ios::trunc);
    for(int i=0;i< this-> elecount;i++){
        if(i != 0){
            fout << " ";
        }
        fout << this->matrixelems[i];
    }
    fout<<endl;
    fout.close();
}

Page::Page(string entity, int pageIndex,int pType){
    if(pType == 1){
        this->pType = pType;
        mPage(entity,pageIndex);
    }
    else if(pType == 0){
        this->pType = pType;
        tPage(entity,pageIndex);
    }
}

void Page::tPage(string tableName,int pageIndex){
    logger.log("Page::Page");
    this->entity = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->entity + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount=table.columnCount;
    uint maxRowCount=table.maxRowsPerBlock;
    vector<int> row(columnCount,0);
    this->trow.assign(maxRowCount,row);
    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++){
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++){
            fin >> number;
            this->trow[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}


void Page::mPage(string matrixName,int pageIndex){
    logger.log("Page::Page");
    this->entity = matrixName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->entity + "_Page" + to_string(pageIndex);
    Matrix matrix = *matrixCatalogue.getMatrix(matrixName);
    uint maxelemcnt = matrix.maxelemperblock;
    this->matrixelems.assign(maxelemcnt,0);
    ifstream fin(pageName, ios::in);
    this->elecount = matrix.elemperblockcount[pageIndex];
    int number;
    for (int i=0;i< this->elecount;i++){
        fin >> number;
        this->matrixelems[i] = number;
    }
    fin.close();
}