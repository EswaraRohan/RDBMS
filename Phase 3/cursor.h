#include"bufferManager.h"
/**
 * @brief The cursor is an important component of the system. To read from a
 * table, you need to initialize a cursor. The cursor reads rows from a page one
 * at a time.
 *
 */
class Cursor{
    public:
    Page page;
    int pageIndex;
    string tableName;
    int pagePointer;
    int tabIndex = 1;

    public:
    Cursor(string tableName, int pageIndex);
    Cursor(string tableName, int tabIndex, int pageIndex);
    vector<int> getNext();
    void nextPage(int pageIndex);
    vector<int> getCurrPage();
};