#define dxe_h
using namespace std;

FILE *readInput(char *argv[], const char *ptr);

void inputErrorCheck(int argc, char *argv[]);

int getEndAddress(FILE **fptr);

void createOutFiles(char *argv[],ofstream &sic,ofstream &lis);

void createInstructionMap(map<int,string> &imap);

void readSym(FILE **fptr,vector<string> &symbols, vector<int> &addresses, 
                vector<char> &flags, vector<string> &literals, vector<int> &lens,
                vector<string> &names, vector<int> &litAddr);

void readObj(FILE **fptr, vector<string> &symbols, vector<int> &addresses, vector<char> &flags,
                vector<string> &literals, vector<int> &lens, vector<string> &names, vector<int> &litAddr, ofstream &sic, ofstream &lis);

int main(int argc,char *argv[]);
