#include "global.h"

BufferManager::BufferManager()
{
    logger.log("BufferManager::BufferManager");
}
/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::getPage(string tableName, int pageIndex)
{
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    if (this->inPool(pageName))
        return this->getFromPool(pageName);
    else
        return this->insertIntoPool(tableName, pageIndex);
}
/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName 
 * @return true 
 * @return false 
 */
bool BufferManager::inPool(string pageName)
{
    logger.log("BufferManager::inPool");
    for (auto page : this->pages)
    {
        if (pageName == page.pageName)
            return true;
    }
    return false;
}
/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName 
 * @return Page 
 */
Page BufferManager::getFromPool(string pageName)
{
    logger.log("BufferManager::getFromPool");
    for (auto page : this->pages)
        if (pageName == page.pageName)
            return page;
}
/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure. 
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::insertIntoPool(string tableName, int pageIndex)
{
    logger.log("BufferManager::insertIntoPool");
    Page page(tableName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}
/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void BufferManager::writePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("BufferManager::writePage");
    Page page(tableName, pageIndex, rows, rowCount);
    page.writePage();
}
/**
 * @brief Deletes file names fileName
 *
 * @param fileName 
 */
void BufferManager::deleteFile(string fileName)
{
    
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
        else logger.log("BufferManager::deleteFile: Success");
}
/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param tableName 
 * @param pageIndex 
 */
void BufferManager::deleteFile(string tableName, int pageIndex)
{
    logger.log("BufferManager::deleteFile");
    string fileName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    this->deleteFile(fileName);
}

void BufferManager::Swap(string tableName, int pageIndex, int elementIndex , int val){
    logger.log("BufferManager::Swap");
    Page page(tableName, pageIndex);
    page.matrixelems[elementIndex] = val;
}

void BufferManager::updatePage(string tableName,int pageIndex, vector<int> rows, int count){
    string fileName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    ofstream fout(fileName, ios::trunc);
    for (int i=0;i<count;i++){
        if (i!=0){
            fout << " ";
        }
        fout << rows[i];
    }
        fout << endl;
    fout.close();
}

void BufferManager::writePage(string tableName, int pageIndex, vector<int> rows, int count){
    logger.log("BufferManager::writeMatrixPage");
    Page page(tableName, pageIndex,rows,count);
    page.writeMatrixPage();
}

Page BufferManager::getPage(string tableName, int pageIndex,int entity){
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    if (this->inPool(pageName))
        return this->getFromPool(pageName);
    else
        if(entity == 0)
            return this->insertIntoPool(tableName, pageIndex,0);
        else if (entity == 1)
            return this->insertIntoPool(tableName,pageIndex,1);
}

Page BufferManager::insertIntoPool(string tableName, int pageIndex,int entity){
    logger.log("BufferManager::insertIntoPool");
    Page page(tableName,pageIndex,entity);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}

vector<int> BufferManager::readPage(string tableName,int pageIndex){
    string pageName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    ifstream fin(pageName,ios::in);
    string line,word;
    vector<int> temp;
    while(getline(fin,line)){
        stringstream s(line);
        string num;
        while (getline(s,num,' ')){
            temp.push_back(stoi(num));
        }
    }
    return temp;
}