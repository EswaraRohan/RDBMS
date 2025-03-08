#include"logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files. 
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs. 
 *</p>
 */

class Page{

    string tableName;
    string pageIndex;
    int columnCount;
    int rowCount;
    int pType;
    string entity;
    int elecount;
    vector<vector<int>> rows;
    vector<vector<int>> trow;

    public:
    vector<int> matrixelems;
    string pageName = "";
    Page();
    Page(string tableName, int pageIndex);
    Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
    Page(string entity,int pageIndex,vector<int> elements,int elecount);
    Page(string entity, int pageIndex,int pType);
    void tPage(string tableName,int pageIndex);
    void mPage(string matrixName,int pageIndex);

    vector<int> getRow(int rowIndex);
    void writePage();
    void writeMatrixPage();
};