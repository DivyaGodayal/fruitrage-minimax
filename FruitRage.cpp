#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include<string>
#include<queue>
#include<time.h>
#include<algorithm>
using namespace std;

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int N;
int P;
float T;
int eVal = 0;
int rowResult = -1;
int colResult = -1;
int bestValue = -100000;
int worstValue = 100000;
int depthRoot;
clock_t endTime;
clock_t startTime = clock();
double gracePeriod = 3;
int breadth = 0;
int maxInitialDepth = 1000;
int maxDepth = 1000;

class BoardState{
public:
   int value;
   vector< vector<int> > board ;
   int row;
   int col;
   int MaxValue;
};


vector<BoardState> pq;
int Evaluate(int row, int col,vector< vector<int> > &b,vector< vector<int> > &child)
{
   int val = 1;
   int fruitVal = b[row][col];
   b[row][col] = -1;
   child[row][col] = -1;
   eVal++;

   for (int i = row, j = col + 1; j < N; j++)
   {
       if (b[i][j] == fruitVal)
           val += Evaluate(i,j,b,child);
       else
           break;
   }

   for (int i = row, j = col - 1; j >= 0; j--)
   {
       if (b[i][j] == fruitVal)
           val += Evaluate(i,j,b,child);
       else
           break;
   }

   for (int i = row - 1, j = col; i >= 0; i--)
   {
       if (b[i][j] == fruitVal)
           val += Evaluate(i,j,b,child);
       else
           break;
   }

   for (int i = row + 1, j = col; i < N; i++)
   {
       if (b[i][j] == fruitVal)
           val += Evaluate(i,j,b,child);
       else
           break;
   }

   return val;
}

void redraw(vector <vector<int> > &a)
{
   for (int col = 0; col < N; col++)
   {
       //TODO:
       // level is to maintain level till which * has already replaced the fruits.
       // with effect of gravity
       // So there is no need to apply gravity to those cells.
       int rowUpdate = N - 1;
       for (int row = N - 1; row >= 0; row--)
       {
           if (a[row][col] != -1)
           {
               a[rowUpdate][col] = a[row][col];
               rowUpdate--;
           }
       }

       for (int row = rowUpdate; row >= 0; row--)
           a[row][col] = -1;
   }
}

void setMaxDepths() {
   int breadth = pq.size();

   if (breadth > 100) {
       maxInitialDepth = 1;
       maxDepth = 1;
   } else if (breadth > 75) {
       maxInitialDepth = 2;
       maxDepth = 2;
   } else if (breadth > 50) {
       maxInitialDepth = 2;
       maxDepth = 3;
   } else if (breadth > 20) {
       maxInitialDepth = 2;
       maxDepth = 4;
   } else if (breadth > 15) {
       maxInitialDepth = 2;
       maxDepth = 5;
   } else if (breadth > 10) {
       maxInitialDepth = 3;
       maxDepth = 6;
   } else {
       maxInitialDepth = 3;
       maxDepth = 1000;
   }
}

void setGracePeriod() {
   breadth = pq.size();

   if (breadth > 75)
       gracePeriod = 275;
   else if (breadth > 70)
       gracePeriod = 260;
   if (breadth > 65)
       gracePeriod = 245;
   if (breadth > 60)
       gracePeriod = 230;
   if (breadth > 55)
       gracePeriod = 210;
   if (breadth > 50)
       gracePeriod = 190;
   else if (breadth > 45)
       gracePeriod = 160;
   else if (breadth > 40)
       gracePeriod = 135;
   else if (breadth > 35)
       gracePeriod = 115;
   else if (breadth > 30)
       gracePeriod = 105;
   else if (breadth > 25)
       gracePeriod = 95;
   else if (breadth > 20)
       gracePeriod = 75;
   else if (breadth > 15)
       gracePeriod = 45;
   else if (breadth > 10)
       gracePeriod = 25;
   else if (breadth > 7)
       gracePeriod = 5;
   else
       gracePeriod = 3;
}

bool timeout() {
   if (colResult == -1)
       return false;

   double timeTaken = double(clock() - startTime) / CLOCKS_PER_SEC;
   double timeRemaining = T - timeTaken;
   return timeRemaining < gracePeriod;
}


int miniMaxTillTwolevels(bool maximizingPlayer, vector <vector<int> > b,int depth)
{
   if (timeout())
       return 0;

   //copying the parent vector to child cummulative vector
   //now this cummulative vector could be used to keep a track of all the children already created
   vector <vector<int> > childrenCummulativeBoard(N, vector<int>(N));
   childrenCummulativeBoard = b;
   if (depth > 0) {
       if (maximizingPlayer)
       {
           int v = -100000;
           bool endCondition = false;
           int i;
           int j;
           for (i = 0; i < N; i++)
           {
               for (j = 0; j < N; j++)
               {

                   if (b[i][j] != -1 && childrenCummulativeBoard[i][j] != -1)
                   {

                       if(depth == 1)
                           breadth++;
                       endCondition = true;
                       vector <vector<int> > c(N, vector<int>(N));
                       for (int p = 0; p < N; p++)
                       {
                           for (int q = 0; q < N; q++)
                           {
                               c[p][q] = b[p][q];
                           }
                       }
                       int move = pow(Evaluate(i, j, c, childrenCummulativeBoard),2);
                       redraw(c);
                       int val = miniMaxTillTwolevels(false, c, depth-1);

                       if(depth == 1)
                       {

                           BoardState b = BoardState();
                           b.value = val + move;
                           b.board = c;
                           b.row = i;
                           b.col = j;
                           b.MaxValue = move;
                           pq.push_back(b);
                       }
                       if(val+move > v)
                       {
                           rowResult = i;
                           colResult = j;
                       }
                       v = max(val+move,v);

                   }
                   if (timeout())
                       return endCondition ? v : 0;
               }
           }

           return endCondition ? v : 0;
       }
       else
       {
           bool endCondition = false;
           int v = 100000;
           int i,j;
           for (i = 0; i < N; i++)
           {
               for (j = 0; j < N; j++)
               {
                   if (b[i][j] != -1  && childrenCummulativeBoard[i][j] != -1)
                   {
                       endCondition = true;
                       vector < vector<int> >  c(N, vector<int>(N));
                       for (int p = 0; p < N; p++)
                       {
                           for (int q = 0; q < N; q++)
                           {
                               c[p][q] = b[p][q];
                           }
                       }
                       int move = pow(Evaluate(i, j, c, childrenCummulativeBoard),2);
                       redraw(c);
                       int val = miniMaxTillTwolevels(true, c, depth-1);
                       v = min(val-move,v);
                   }

                   if (timeout())
                       return endCondition ? v : 0;
               }
           }
           return endCondition ? v : 0;
       }
   }
   return 0;
}

int miniMax(bool maximizingPlayer, vector <vector<int> > b,int valuation, int depth,  int alpha, int beta)
{
   if (timeout())
       return 0;
   //copying the parent vector to child cummulative vector
   //now this cummulative vector could be used to keep a track of all the children already created
   if (depth >= 0) {
       vector <vector<int> > childrenCummulativeBoard(N, vector<int>(N));
       childrenCummulativeBoard = b;
       if (maximizingPlayer)
       {
           int v = -100000;
           bool endCondition = false;
           int i;
           int j;
           for (i = 0; i < N; i++)
           {
               for (j = 0; j < N; j++)
               {
                   if (b[i][j] != -1 && childrenCummulativeBoard[i][j] != -1)
                   {
                       endCondition = true;
                       vector <vector<int> > c(N, vector<int>(N));

                       for (int p = 0; p < N; p++)
                       {
                           for (int q = 0; q < N; q++)
                           {
                               c[p][q] = b[p][q];
                           }
                       }
                       int move = pow(Evaluate(i, j, c,childrenCummulativeBoard),2);
                       redraw(c);
                       int val = miniMax(false, c, valuation+move, depth-1, alpha,beta);

                       v = max(val,v);
                       alpha = max(v,alpha);

                       if (beta <= alpha)
                           return v; // beta cutoff

                   }


                   if (timeout())
                       return endCondition ? v : valuation;
               }
           }

           return endCondition ? v : valuation;
       }
       else
       {
           bool endCondition = false;
           int v = 100000;
           int i,j;
           for (i = 0; i < N; i++)
           {
               for (j = 0; j < N; j++)
               {
                   if (b[i][j] != -1 && childrenCummulativeBoard[i][j] != -1)
                   {
                       endCondition = true;
                       vector < vector<int> >  c(N, vector<int>(N));
                       for (int p = 0; p < N; p++)
                       {
                           for (int q = 0; q < N; q++)
                           {
                               c[p][q] = b[p][q];
                           }
                       }
                       int move = pow(Evaluate(i, j, c, childrenCummulativeBoard),2);
                       redraw(c);
                       int val = miniMax(true, c, valuation-move, depth-1, alpha, beta);

                       v = min(val,v);
                       beta = min(v,beta);
                       if (beta <= alpha)
                           return v;// beta cutoff

                   }

                   if (timeout())
                       return endCondition ? v : valuation;
               }
           }

           return endCondition ? v : valuation;
       }
   }
   return valuation;
}

int miniMaxProgressive(bool maximizingPlayer, vector <vector<int> > b, int alpha, int beta){

   int winner = bestValue;

   miniMaxTillTwolevels(true, b, 1);

   setMaxDepths();
   setGracePeriod();

   if(maxInitialDepth > 1)
   {	//Calling minimax to depth 2 and then reordering the nodes

       for (int i = 0; i < pq.size(); i++) {
           vector <vector<int> > c(N, vector<int>(N));

           for (int p = 0; p < N; p++)
           {
               for (int q = 0; q < N; q++)
               {
                   c[p][q] = pq[i].board[p][q];
               }
           }
           pq[i].value += miniMaxTillTwolevels(false, c, 1);
           if (pq[i].value > winner) {
               winner = pq[i].value;
               rowResult = pq[i].row;
               colResult = pq[i].col;
           }

           if (timeout()) {
               return winner;
           }
       }
   }

   if (timeout()) {
       return winner;
   }

   std::sort (pq.begin(), pq.end(), [](const BoardState& lhs, const BoardState& rhs)
              {
                  return lhs.value < rhs.value;
              });


   int depth = maxDepth;
   BoardState bd = BoardState();
   //Donot repeat the computation if the depth is 2 or 1. Return the best move you have till now.
   bd = pq[pq.size()-1];
   if(maxDepth == 1 || maxDepth == 2)
   {
       rowResult = bd.row;
       colResult = bd.col;
       winner = bd.value;
       return winner;
   }


   winner  = -100000;
   while(pq.size() > 0 && !timeout())
   {
       BoardState bd = BoardState();
       bd = pq[pq.size()-1];
       b = bd.board;
       depthRoot = depth-1;
       int valuation = bd.MaxValue;
       int value = miniMax(false, b ,valuation, depthRoot, alpha, beta);
       alpha = max(alpha,value);
       if(value>winner)
       {
           winner = value;
           rowResult = bd.row;
           colResult = bd.col;

       }
       pq.pop_back();
   }

   return winner;
}

int main()
{
   std::vector< std::vector<int> >  b;

   ifstream inputFile;
   ofstream outputFile;

   inputFile.open("./input.txt");
   if (inputFile.is_open())
   {
       string line;

       getline(inputFile, line);       // width and height of square board
       N = stoi(line);

       getline(inputFile, line);       // number of fruit types
       P = stoi(line);

       getline(inputFile, line);       // remaining time in seconds
       T = stof(line);

       b = vector< vector<int> >(N, vector<int>(N));

       for (int i = 0; i < N; i++)
       {
           getline(inputFile, line);
           for (int j = 0; j < N; j++)
           {
               if (line[j] == '*')
                   b[i][j] = -1;
               else
                   b[i][j] = line[j] - '0';
           }
       }
       inputFile.close();
   }


   endTime = clock() +(T-10)* CLOCKS_PER_SEC;

   vector < vector<int> > a = b;
   miniMaxProgressive(true, b, bestValue, worstValue);
   eVal = 0;
   Evaluate(rowResult, colResult, a, a);

   outputFile.open("./output.txt", ios::trunc);
   if (outputFile.is_open()) {
       outputFile << alphabet[colResult] << rowResult + 1 << "\n";

       redraw(a);
       for (int i = 0; i < N; i++)
       {
           for (int j = 0; j < N; j++)
           {
               if (a[i][j] == -1)
                   outputFile << "*";
               else
                   outputFile << a[i][j];
           }
           outputFile << "\n";
       }

       outputFile.close();
   }

   return 0;
}
