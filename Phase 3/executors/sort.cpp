#include"global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    if(tokenizedQuery.size()!= 8 && tokenizedQuery.size() != 10 || tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    if(tokenizedQuery.size() == 10)
    {
        int check = 1;
        string buffer_sizein = tokenizedQuery[9];


        for(int j = 0;j<buffer_sizein.length();j++)
        {
            if(tokenizedQuery[9][j] < '0' || tokenizedQuery[9][j] > '9')
            {
                check = 0;
                break;
            }
        }
        if(tokenizedQuery[8] != "BUFFER" || check == 0)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        else
        {
            parsedQuery.buffer_size = stoi(buffer_sizein);
        }
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    //cout<<tokenizedQuery[0];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    string sortingStrategy = tokenizedQuery[7];

    if(sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrategy == "DESC" || sortingStrategy == "DSC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(parsedQuery.buffer_size < 3)
    {
        cout<<"SEMANTIC ERROR: Size of buffer cannot be less than 3!"<<endl;
        return false;
    }

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}

void sortPhase1(Table &tab, Table *resTab){
    logger.log("sortPhase1");
    Cursor curs = tab.getCursor();
    string relName = parsedQuery.sortRelationName;
    string colName = parsedQuery.sortColumnName;
    
    int sortColI = tab.getColumnIndex(colName);    
    int x = parsedQuery.buffer_size - 1;
    int y = (double)tab.blockCount/(double)x;
    y = ceil(y);

    vector<vector<int>> rowsResult(resTab->maxRowsPerBlock, vector<int>(resTab->columnCount,0));

    for(resTab->blockCount = 0; resTab->blockCount < tab.blockCount;)
    {
        multimap<int,vector<int>> rowSorted;
        int sortOrder;
        if(parsedQuery.sortingStrategy == ASC)
        sortOrder = 1;
        else
        sortOrder = -1;

        for(int i = resTab->blockCount; i<min((resTab->blockCount + x),tab.blockCount);i++)
        {
            vector<int> r = curs.getCurrPage();
            while(!r.empty())
            {
                rowSorted.insert({sortOrder*r[sortColI],r});
                r = curs.getCurrPage();
            }
            bool check = tab.getNextPage(&curs);
            if(!check)
            break;
        }
        int numRows = 0;
        auto i = rowSorted.begin();
        while(i != rowSorted.end())
        {
            rowsResult[numRows++] = i++->second;
            if(numRows == resTab->maxRowsPerBlock)
            {
                bufferManager.writePage(resTab->tableName,y,resTab->blockCount,rowsResult,numRows);
                resTab->rowCount = resTab->rowCount + numRows;
                resTab->blockCount += 1;
                resTab->rowsPerBlockCount.emplace_back(numRows);
                numRows = 0;    
            }
        }
        if(numRows)
        {
            bufferManager.writePage(resTab->tableName,y,resTab->blockCount,rowsResult,numRows);
            resTab->rowCount = resTab->rowCount + numRows;
            resTab->blockCount += 1;
            resTab->rowsPerBlockCount.emplace_back(numRows);
            numRows = 0; 
        }
    }

tableCatalogue.insertTable(resTab);

}

void sortPhase2(Table &tab, Table *tabRes)
{
    logger.log("sortPhase2");

    int sortColI = tab.getColumnIndex(parsedQuery.sortColumnName);
    int x = parsedQuery.buffer_size- 1;
    int y = ceil((double)tabRes->blockCount / (double)x);
    int z = x;
    
    int numRows = 0;
    vector<vector<int>> resultantRows(tabRes->maxRowsPerBlock, vector<int>(tabRes->columnCount, 0));

    while (y != 1)
    {
        int next_nr = ceil((double)y / (double)x);
        int blkCnt = 0;
        for (int i = 0; i < y; i =i+ x)
        {
            multimap<int, pair<vector<int>, Cursor*>> RM;
        int sortOrder;
            if(parsedQuery.sortingStrategy == ASC)
        sortOrder = 1;
        else
        sortOrder = -1;
            for (int j = i; j < min(y, i + x); j++)
            {
                Cursor *crs = new Cursor(tabRes->tableName, y, j * z);
                vector<int> row = crs->getCurrPage();
                RM.insert({sortOrder * row[sortColI], {row, crs}});
            }

            while (!RM.empty())
            {
                auto it = RM.begin();
                vector<int> row = it->second.first;
                Cursor *crs = it->second.second;
                RM.erase(it);

                resultantRows[numRows] = row;
                numRows+=1;
                if (numRows == tabRes->maxRowsPerBlock)
                {
                    bufferManager.writePage(tabRes->tableName, next_nr, blkCnt++, resultantRows, numRows);
                    numRows = 0;
                }
                row = crs->getCurrPage();
                
                if (row.empty())
                {
                    if ((crs->pageIndex + 1) % z && (crs->pageIndex + 1) != tabRes->blockCount)
                    {
                        crs->nextPage(crs->pageIndex + 1);
                        row = crs->getCurrPage();
                    }
                    else
                    {
                        delete crs;
                    }
                }
                if (!row.empty())
                {
                    RM.insert({sortOrder * row[sortColI], {row, crs}});
                }
            }

            if (numRows)
            {
                bufferManager.writePage(tabRes->tableName, next_nr, blkCnt, resultantRows, numRows);

                blkCnt = blkCnt+1;
                numRows = 0;
            }
        }
         z = z * x;
        int pageCnt = 0;
        for (pageCnt = 0; pageCnt < tabRes->blockCount; pageCnt++)
        {
            bufferManager.deleteFile(tabRes->tableName, y, pageCnt);
        }
        y = next_nr;
        
    }
}

void executeSORT(){
    logger.log("executeSORT");
    Table tab =   *tableCatalogue.getTable(parsedQuery.sortRelationName);
    Table *resTab = new Table(parsedQuery.sortResultRelationName,tab.columns);

    sortPhase1(tab,resTab);
    sortPhase2(tab,resTab);

    return;
}