#include "global.h"


Matrix::Matrix(){
    logger.log("Matrix::Matrix");
}


Matrix::Matrix(string MatrixName){
    logger.log("Matrix:Matrix");
    this -> sourceFileName = "../data/" + MatrixName + ".csv";
    this -> MatrixName = MatrixName;
}


bool Matrix::load(){
    logger.log("Matrix::load");
    fstream fin(this -> sourceFileName, ios::in);
    string line;
    
    if (this -> blockify()==true)
        return true;
    fin.close();
    return false;
}

bool Matrix::blockify(){
    logger.log("Matrix::blockify");
    ifstream fin(this -> sourceFileName, ios::in);
    string line, word;
    int elemcount=0,colcount=0,rowcount=0,totelems=0;
    vector <int> eleminpage;
    if(getline(fin,line)){
        stringstream s(line);
        string num;
        while(getline(s, num, ',')){
            colcount++;
        }
    }
    this -> matrixdimension = colcount;
    this->maxelemperblock = (uint)((BLOCK_SIZE*1000)/(32));
    this->maxelemperblock = (this->maxelemperblock/3)*3;
    fin.close();
    fin.open(this -> sourceFileName,ios::in);
    while (getline(fin, line)){
        stringstream s(line);
        for (int i=0;i< this->matrixdimension;i++){
            if (!getline(s, word, ','))
                return false;
            totelems++;
        }
    } 
    fin.close();
    fin.open(this->sourceFileName,ios::in);;
    while (getline(fin, line)){
        stringstream s(line);
        for (int i=0;i< this->matrixdimension;i++){
            if (!getline(s, word, ','))
                return false;
            eleminpage.push_back(stoi(word));
            elemcount++;

            if(elemcount == this->maxelemperblock){
                bufferManager.writePage(this->MatrixName,this->blockcnt,eleminpage,elemcount);
                this->blockcnt+=1;
                this->elemperblockcount.emplace_back(elemcount);
                elemcount=0;
                eleminpage.clear();
                
            }

        }
        rowcount++;

    }

    if(elemcount){
        bufferManager.writePage(this->MatrixName,this->blockcnt,eleminpage,elemcount);
        this->blockcnt+=1;
        this->elemperblockcount.emplace_back(elemcount);
        elemcount=0;
    }

    this->rowsnum = rowcount;
    if(rowcount == 0){
        return false;
    }


    return true;

}

void Matrix::print(){
    uint ctr=min((uint)PRINT_COUNT,this->rowsnum);
    Cursor cursor(this->MatrixName,0,1);
    int eleind=0;
    int pgind=0;
    vector<int> row;
    vector<int> temp;
    temp = bufferManager.readPage(this->MatrixName,pgind);   
        for(int i=0;i<ctr*matrixdimension;i++){
            if(i%matrixdimension == 0){
                cout << "\n";
            }
            cout << temp[eleind] << " ";
            eleind+=1;
            if(eleind == temp.size()){
                pgind+=1;
                temp=bufferManager.readPage(this->MatrixName,pgind);
                eleind=0;
            }
        }
        cout << "\n";
}

string Matrix::matrixname(){
    string str = this->MatrixName;
    return str;
}

void Matrix::clearfile(){
    bufferManager.deleteFile(this->sourceFileName);
}

void Matrix::makePermanent(string MatName){
    logger.log("Matrix::makePermanent");   
    if (!this->isPermanent()){
        bufferManager.deleteFile(this->sourceFileName);
    }
    string newSourceFile = "../data/" + MatName + ".csv";
    ofstream fout(newSourceFile, ios::out);
        uint ctr = this->rowsnum;
        Cursor cursor(this->MatrixName,0,1);
        int eleind=0;
        int pgind=0;
        vector<int> temp;
        temp = bufferManager.readPage(this->MatrixName,pgind);

        for(int i=0;i<ctr*this->matrixdimension;i++){
            fout << temp[eleind];
            if((i+1)%matrixdimension == 0){
                fout << "\n";
            }
            else{
                fout << ", ";
            }
            eleind+=1;
            if(eleind == temp.size()){
                pgind+=1;
                temp = bufferManager.readPage(this->MatrixName,pgind);
                eleind=0;
            }
        }
    fout.close();
}

void Matrix::makePermanenthere(string MatName){
    string newSourceFile = "../data/" + MatName + ".csv";
    ofstream fout(newSourceFile, ios::out);
        uint ctr = this->rowsnum;
        Cursor cursor(this->MatrixName,0,1);
        int eleind=0;
        int pgind=0;
        vector<int> temp;
        temp = bufferManager.readPage(this->MatrixName,pgind);

        for(int i=0;i<ctr*this->matrixdimension;i++){
            fout << temp[eleind];
            if((i+1)%matrixdimension == 0){
                fout << "\n";
            }
            else{
                fout << ", ";
            }
            eleind+=1;
            if(eleind == temp.size()){
                pgind+=1;
                temp = bufferManager.readPage(this->MatrixName,pgind);
                eleind=0;
            }
        }
    fout.close();
    return;
}


bool Matrix::isPermanent(){
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->MatrixName + ".csv")
        return true;
    return false;
}

void Matrix::CROSS_TRANSPOSE(){
    for(int i=0;i<this->rowsnum;i++){
        for(int j=i;j<this->matrixdimension;j++){

            int pgind1 = (i*(this->matrixdimension)+j)/(this->maxelemperblock);
            int pgind2 = (j*(this->matrixdimension)+i)/(this->maxelemperblock);

            Cursor c1(this->MatrixName,pgind1,1);
            Cursor c2(this->MatrixName,pgind2,1);

            vector<int> temp1 = bufferManager.readPage(this->MatrixName,pgind1);
            vector<int> temp2 = bufferManager.readPage(this->MatrixName,pgind2);
            
            int var1 = (i*(this->matrixdimension)+j)%(this->maxelemperblock);
            int var2 = (j*(this->matrixdimension)+i)%(this->maxelemperblock);
            int var3 = temp1[var1];
            int var4 = temp2[var2];
            
            temp1[var1] = var4;
            if(pgind2 != pgind1){
                temp2[var2] = var3;
            }
            else{
                temp1[var2] = var3;
            }

            bufferManager.updatePage(this->MatrixName, pgind1, temp1, temp1.size());

            if(pgind2 != pgind1){
                bufferManager.updatePage(this->MatrixName, pgind2, temp2, temp2.size());
            }

        }
    }
    return;
}

void Matrix::unload(){
    logger.log("Matrix::~unload");
    for (int pageCounter=0;pageCounter < this->blockcnt;pageCounter++)
        bufferManager.deleteFile(this->MatrixName,pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}


Cursor Matrix::getCursor(){
    logger.log("Matrix::getCursor");
    Cursor cursor(this->MatrixName,0,1);
    return cursor;
}