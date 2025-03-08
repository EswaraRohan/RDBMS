
class Matrix
{

public:
    string sourceFileName = "";
    string MatrixName = "";
    uint matrixdimension=0;
    uint blockcnt=0;
    uint rowsnum = 0;
    uint maxelemperblock = 0;
    vector <uint> elemperblockcount;
    bool sparse = false;

        
    bool blockify();
    Matrix();
    Matrix(string MatrixName);
    bool load();
    void print();
    void CROSS_TRANSPOSE();
    void makePermanent(string MatName);
    bool isPermanent();
    Cursor getCursor();
    void unload();
    string matrixname();
    void makePermanenthere(string MatName);
    void clearfile();

template <typename T>
void writeRow(vector<T> row, ostream &fout)
{
    logger.log("Matrix::printRow");
    for (int columnCounter = 0; columnCounter < row.size(); columnCounter++)
    {
        if (columnCounter != 0)
            fout << ", ";
        fout << row[columnCounter];
    }
    fout << endl;
}

template <typename T>
void writeRow(vector<T> row)
{
    logger.log("Matrix::printRow");
    ofstream fout(this->sourceFileName, ios::app);
    this->writeRow(row, fout);
    fout.close();
}
};