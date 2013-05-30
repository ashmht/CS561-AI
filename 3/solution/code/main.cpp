#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <time.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;
typedef std::string str;

float iteration = 0.0f;

/*
	Data structure for literal Xmn
*/
class Literal
{
	public:
		int guest_number;
		int table_number;
		bool isNegated;
		bool model_value;
	public:
		Literal()
		{

		}

		Literal(int _gNumber, int _tNumber,bool _isNegated)
		{
			guest_number = _gNumber;
			table_number = _tNumber;
			isNegated = _isNegated;
		}
};

//Define the Clause
typedef std::vector<Literal> Clause;
typedef std::map<int, Clause> Sentence;
typedef std::multimap<int,int> resolutionHistory;
typedef std::vector<int> INTVECTOR;

/*
  Output file Class
*/
class outputFile
{
	private:
	    ofstream    oFile;
	    str fileName;

	public:
	    void openFile(str _fileName)
	        {
		      fileName = _fileName;
		      oFile.open(fileName.c_str());
	        }

	    void writeToFile(str data)
	        {
	         if (oFile.is_open())
	            {
	            oFile << data;
	            }
	        }

	    void closeFile()
	        {
	        oFile.close();
	        }
};

/*
	Class that contains data members and 
	member functions to perform PL-Resolution
*/
class PL_RESOLVE
{
	private:
		Sentence::reverse_iterator ritS;
		Clause::iterator c1Itr,r1Itr;
		Clause::iterator c2Itr,r2Itr;

		Sentence newSentence;
		resolutionHistory rHistory;

		outputFile oFile;
		outputFile opFile;

	public:
		PL_RESOLVE()
		{
			oFile.openFile("PL_Resolution.txt");
		}

		bool pl_resolution(Sentence _s)
		{
			rHistory.clear();
			newSentence.clear();
			newSentence = _s;

			for (int i = 1; i <= newSentence.size(); ++i)
			{
				for (int j = 1; j <= newSentence.size(); ++j)
				{
					//printHistory();
					//cout << "----------------"<<endl;
					string printData;
					stringstream ss;

					/*
					ss << i;
					printData = "Resolving C"+ss.str();
					oFile.writeToFile(printData);
					ss.str("");

					ss << j;
					printData = " and C"+ss.str()+"\n";

					oFile.writeToFile(printData);
					ss.str("");
					*/

					if ( (i != j) && !isResolved(i,j))
					{
						//Both Ci and Cj are not same and they have not been resolved earlier.
						//so try resolving now.
						std::pair<bool,Clause> rValue = pl_resolve(i,newSentence[i],j,newSentence[j]);

						if (rValue.first == true)
						{
							//oFile.writeToFile("Resolution Successful!\n");
							if (!(rValue.second).empty())
							{
								if (!inKB(rValue.second))
								{
									ritS = newSentence.rbegin();
									int x = ritS->first+1;
									newSentence[x] = rValue.second;

									rHistory.insert(pair<int, int>(x, i));
									rHistory.insert(pair<int, int>(i, x));
									rHistory.insert(pair<int, int>(x, j));
									rHistory.insert(pair<int, int>(j, x));

									printSentence();

									/*
									ss << x;
									printData = "Added C"+ss.str()+" to KB\n";
									oFile.writeToFile(printData);
									ss.str("");
									*/
								}
								else
								{
									//oFile.writeToFile("Already in KB!\n");
								}
							}
							else
							{
								//oFile.writeToFile("Resolution returned Empty clause.\n");
								return false;
							}
						}
						else
						{
							//oFile.writeToFile("Resolution failed!!\n");
						}
					}
					else
					{
						/*
						ss << i;
						printData = "Either C"+ss.str();
						oFile.writeToFile(printData);
						ss.str("");

						ss << j;
						printData = " == C"+ss.str();

						oFile.writeToFile(printData);
						ss.str("");

						oFile.writeToFile(" or Already resloved.\n");
						*/
					}
				}
			}

			return true;
		}

		void printHistory()
		{
			resolutionHistory::iterator hisItr;
			if (!rHistory.empty())
			{
				for ( hisItr = rHistory.begin(); hisItr != rHistory.end(); ++hisItr)
				{
					cout << "C"<<hisItr->first<<" - C"<<hisItr->second<<endl;
				}
			}
		}

		bool inKB(Clause _c)
		{
			Clause c;
			for (int i = 1; i <= newSentence.size(); ++i)
			{
				int equalCount = 0;

				c = newSentence[i];

				if(c.size() == _c.size())
				{
					for (Clause::iterator j = _c.begin(); j != _c.end(); ++j)
					{
						for (Clause::iterator k = c.begin(); k != c.end(); ++k)
						{
							if ((*k).guest_number == (*j).guest_number && (*k).table_number == (*j).table_number && (*k).isNegated == (*j).isNegated)
							{
								equalCount++;
							}
						}
					}

					if (equalCount == _c.size())
					{
						return true;
					}
				}
			}

			return false;
		}

		bool isResolved(int _c1Number,int _c2Number)
		{
			pair<resolutionHistory::iterator, resolutionHistory::iterator> ppp;

			// equal_range(_c1Number) returns pair<iterator,iterator> representing the range
	   		// of element with key _c1Number
	   		ppp = rHistory.equal_range(_c1Number);

	   		// Loop through range of maps of key _c1Number
	   		for (resolutionHistory::iterator i = ppp.first; i != ppp.second; ++i)
	   		{
	   			if ((*i).second == _c2Number)
	   			{
	   				return true;
	   			}
	   		}

	   		// equal_range(_c2Number) returns pair<iterator,iterator> representing the range
	   		// of element with key _c2Number
	   		ppp = rHistory.equal_range(_c2Number);

	   		// Loop through range of maps of key _c1Number
	   		for (resolutionHistory::iterator i = ppp.first; i != ppp.second; ++i)
	   		{
	   			if ((*i).second == _c1Number)
	   			{
	   				return true;
	   			}
	   		}

	   		return false;
		}

		//Resolve two clauses Ci and Cj
		std::pair<bool,Clause> pl_resolve(int _c1Number,Clause _c1,int _c2Number,Clause _c2)
		{
			//Resolution Status
			bool canResolve = false;
			int complementaryLiteralCount = 0;
			Clause cTemp;

			//Check if a a literal in Ci has a complementary literal in Cj
			for (c1Itr = _c1.begin(); c1Itr != _c1.end(); ++c1Itr)
			{
				for (c2Itr = _c2.begin(); c2Itr != _c2.end(); ++c2Itr)
				{
					if(((*c1Itr).guest_number == (*c2Itr).guest_number) && ((*c1Itr).table_number == (*c2Itr).table_number) && (((*c1Itr).isNegated == true && (*c2Itr).isNegated == false) || ((*c1Itr).isNegated == false && (*c2Itr).isNegated == true)))
					{
						canResolve = true;
						complementaryLiteralCount++;
						if (complementaryLiteralCount > 1)
						{
							canResolve = false;
							goto stopLiteralCheck;
						}
						r1Itr = c1Itr;
						r2Itr = c2Itr;
					}
				}
			}			

		stopLiteralCheck:
			if (!canResolve)
			{
				//We tried resolving Ci and Cj so put an entry.
				rHistory.insert(pair<int, int>(_c1Number, _c2Number));
				rHistory.insert(pair<int, int>(_c2Number, _c1Number));

				return std::make_pair(false, cTemp);;
			}

			for (c1Itr = _c1.begin(); c1Itr != _c1.end(); ++c1Itr)
			{
				if(r1Itr == c1Itr)
				{
					continue;
				}
				else
				{
					cTemp.push_back((*c1Itr));
				}
			}
			for (c2Itr = _c2.begin(); c2Itr != _c2.end(); ++c2Itr)
			{
				if(r2Itr == c2Itr)
				{
					continue;
				}
				else
				{
					cTemp.push_back((*c2Itr));
				}
			}

			cTemp = sanitizeClause(cTemp);

			//The Clause Ci and Cj has been resolved.
			//Add an entry in the resolution history
			rHistory.insert(pair<int, int>(_c1Number, _c2Number));
			rHistory.insert(pair<int, int>(_c2Number, _c1Number));

			return std::make_pair(true, cTemp);
		}

		Clause sanitizeClause(Clause _c)
		{
			Clause cTemp;

			for (Clause::iterator i = _c.begin(); i != _c.end(); ++i)
			{
				bool canAdd2Clause = true;
				for (Clause::iterator j = _c.begin(); j != _c.end(); ++j)
				{
					if (i == j)
					{
						continue;
					}
					if((*i).guest_number == (*j).guest_number && (*i).table_number == (*j).table_number && (*i).isNegated == (*j).isNegated)
					{
						//Both Li and Lj are exactly the same.
						//Do not add this to cTemp.
						canAdd2Clause = false;
						break;
					}
				}

				if (canAdd2Clause)
				{
					cTemp.push_back(*i);
				}
			}

			return cTemp;
		}

		void printClause(Clause c)
		{
			for (Clause::iterator i = c.begin(); i != c.end(); ++i)
				{
					if((*i).isNegated == true)
					{
						cout<<"-X"<<(*i).guest_number+1<<(*i).table_number+1<<" v ";
					}
					else
					{
						cout<<"X"<<(*i).guest_number+1<<(*i).table_number+1<<" v ";
					}
				}
				cout << endl;
		}

		void printSentence()
		{
			Sentence::reverse_iterator ritS;
			stringstream sPrintData;
			opFile.openFile("Clauses.txt");

			ritS = newSentence.rbegin();

			sPrintData << ritS->first;
			str printData = "Total Number of Clauses generated: "+sPrintData.str();
			opFile.writeToFile(printData);
			sPrintData.str("");

			opFile.writeToFile("\nList of Clauses: \n");

			Sentence::iterator itS;
			for ( itS = newSentence.begin(); itS != newSentence.end(); ++itS)
			{
				sPrintData << itS->first;
				printData = "Clause "+sPrintData.str()+" :: ";
				opFile.writeToFile(printData);
				sPrintData.str("");
				Clause cc = itS->second;
				for (Clause::iterator i = cc.begin(); i != cc.end(); ++i)
				{
					sPrintData << ((*i).guest_number+1);
					sPrintData << ((*i).table_number+1);

					if((*i).isNegated == true)
					{
						printData = "-X"+sPrintData.str()+" ";
					}
					else
					{
						printData = "X"+sPrintData.str()+" ";
					}
					opFile.writeToFile(printData);
					sPrintData.str("");
				}
				opFile.writeToFile("\n");
			}
			opFile.closeFile();
		}

		str itos(int i)
		{
			std::stringstream ss;
			ss << i;
			return ss.str();
		}

		~PL_RESOLVE()
		{
			oFile.closeFile();
		}
};

class WALK_SAT
{
	private:
		int p;
		int M,N;
		INTVECTOR unsatisfiedClause;
		outputFile oFile;

	public:
		int max_flips;
		Sentence s;
		WALK_SAT()
		{
			oFile.openFile("walkSAT.txt");
		}

		void setWalkSATParam(Sentence _s,int _p,int _maxFlips,int _M,int _N)
		{
			s.clear();
			s = _s;
			p = _p;
			max_flips = _maxFlips;
			M = _M;
			N = _N;
			unsatisfiedClause.clear();		}

		bool doWalkSAT()
		{
			//Get a random model
			bool Model[M][N];
			srand(time(0));

			// oFile.writeToFile("Generating random model...\n");

			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					//Generate values [0,1]
					if(rand() % 2 == 0)
					{
						// string printData = "X"+itos(i+1)+itos(j+1)+" = true\n";
						// oFile.writeToFile(printData);
						Model[i][j] = true;
					}
					else
					{
						// string printData = "X"+itos(i+1)+itos(j+1)+" = false\n";
						// oFile.writeToFile(printData);
						Model[i][j] = false;
					}
				}
			}

			// oFile.writeToFile("Random model generation complete.\nStaring max_flips iteration.\n");

			for (int i = 1; i <= max_flips; ++i)
			{
				
				// string printData = "iteration "+itos(i)+": \n";
				// oFile.writeToFile(printData);
				// oFile.writeToFile("Checking for model satisfiability...\n");
				
				std::pair<bool,INTVECTOR> rValue = checkClausesSatisfied((bool*)Model);
				if(rValue.first)
				{
					// oFile.writeToFile("Satisfiable model found.\n");
					iteration += i;
					return true;
				}
				else
				{
					// oFile.writeToFile("Model not satisfiable.\n");
					//Randomly select a clause from the list of Unsatisfied Clauses
					int x = rand() % (rValue.second).size();
					int sentenceIndex = (rValue.second).at(x);
					Clause c = s[sentenceIndex];
					// oFile.writeToFile("Random unsatisfiable clause selected : C"+itos(sentenceIndex)+"\n");
					//With probability p select Random walk or Min-Conflicts
					int xx = rand() % 100 +1;
					if (xx <= p)
					{
						// oFile.writeToFile("Performing random walk\n");
						//Random Walk
						//Select a random symbol from the clause
						int y = rand() % c.size();
						// oFile.writeToFile("Flipping literal at position "+itos(y)+"\n");
						Literal l = c.at(y);

						//Flip the value in the model assocated with the symbol
						// oFile.writeToFile("Model for X"+itos(l.guest_number)+itos(l.table_number)+" has been flipped from ");
						// stringstream ss;
						// ss << Model[l.guest_number][l.table_number];
						// oFile.writeToFile(ss.str());
						// ss.str("");

						Model[l.guest_number][l.table_number] = !Model[l.guest_number][l.table_number];
						
						// ss << Model[l.guest_number][l.table_number];
						// oFile.writeToFile(" to "+ss.str()+"\n");
						// ss.str("");
					}
					else
					{
						//Min-Conflicts
						//For each of the symbols in the clause, flip the model value
						//and check the number of unsatisfied clauses
						//pick the model with the leasr unsatisfied clauses.
						// oFile.writeToFile("Performing Min-Conflicts.\n");
						Literal l;
						int leastUnsatisfiedClauseCount = INT_MAX;
						int k=1;
						for (Clause::iterator i = c.begin(); i != c.end(); ++i)
						{
							//Find the corresponding model value and flip.
							Model[(*i).guest_number][(*i).table_number] = !Model[(*i).guest_number][(*i).table_number];

							//Now get the number of unsatisfied clauses
							std::pair<bool,INTVECTOR> minCon = checkClausesSatisfied((bool*)Model);

							//Revert the model to the old value
							Model[(*i).guest_number][(*i).table_number] = !Model[(*i).guest_number][(*i).table_number];

							// oFile.writeToFile("Literal : "+itos(k)+" Unsatisfied Clause Count :"+itos((minCon.second).size())+"\n");
							 if(leastUnsatisfiedClauseCount > (minCon.second).size())
							 	{
							 		leastUnsatisfiedClauseCount = (minCon.second).size();
							 		l = *i;
							 	}
							 	k++;
						}

						// oFile.writeToFile("Most satisfiable model flip obtained, value = "+itos(leastUnsatisfiedClauseCount)+"\n");

						//Now flip the model value for the symbol with the highest satisfiable clauses
						Model[l.guest_number][l.table_number] = !Model[l.guest_number][l.table_number];
						// oFile.writeToFile("Have picked X"+itos(l.guest_number+1)+itos(l.table_number+1)+" for flipping.\n");
						// oFile.writeToFile("Model flipped , ready for next iteration.\n");
					}
				}

				//Print the current model
				for (int i = 0; i < M; ++i)
				{
					for (int j = 0; j < N; ++j)
					{
						//Generate values [0,1]
						if(Model[i][j])
						{
							string printData = "X"+itos(i+1)+itos(j+1)+" = true\n";
							// oFile.writeToFile(printData);
						}
						else
						{
							string printData = "X"+itos(i+1)+itos(j+1)+" = false\n";
							// oFile.writeToFile(printData);
						}
					}
				}
			}

			return false;
		}

		std::pair<bool,INTVECTOR> checkClausesSatisfied(bool *_model)
		{
			Clause c;
			bool returnStatus = true;
			INTVECTOR unsatisfiedClauseList;

			//Check if the model is true for all the clauses in the KB
			for (int i = 1; i <= s.size(); ++i)
			{
				bool clauseStatus = false;
				c = s[i];
				// oFile.writeToFile("Checking for Clause "+itos(i)+"\n");
				for (Clause::iterator j = c.begin(); j != c.end(); ++j)
				{
					//if at least 1 symbol is true, the entire clause is true
					// oFile.writeToFile("Fetching model...\n");
					if((*j).isNegated)
					{
						// oFile.writeToFile("The Literal is negated.\n");
						(*j).model_value = !getModelValue(_model,(*j).guest_number,(*j).table_number);
						// oFile.writeToFile("Model value for the literal has been inverted!\n");
					}
					else
					{
						// oFile.writeToFile("The Literal is NOT negated.\n");
						(*j).model_value = getModelValue(_model,(*j).guest_number,(*j).table_number);
					}

					if ((*j).model_value)
					{
						// oFile.writeToFile("The entire clause is true...breaking loop.\n");
						clauseStatus = true;
						break;
					}
					else
					{
						clauseStatus = false;
					}
				}

				if(!clauseStatus)
				{
					// oFile.writeToFile("The entire clause is FALSE.\n");
					returnStatus = false;
					unsatisfiedClauseList.push_back(i);
					// oFile.writeToFile("Pushed C"+itos(i)+" as unsatisfied.\n");
				}
			}

			return std::make_pair(returnStatus, unsatisfiedClauseList);
		}

		bool getModelValue(bool *_model,int i,int j)
		{
			bool value = *(_model + i*N + j);
			/*if(value)
			{
				oFile.writeToFile("Model for X"+itos(i+1)+itos(j+1)+" is TRUE\n");
			}
			else{
				oFile.writeToFile("Model for X"+itos(i+1)+itos(j+1)+" is FALSE\n");
			}*/
			return value;
		}

		str itos(int i)
		{
			std::stringstream ss;
			ss << i;
			return ss.str();
		}

		~WALK_SAT()
		{
			oFile.closeFile();
		}
};
 
/*
	Class used to generate an jnstance of Relationship
	between guests.
*/
class InstanceGenerator
{
	private:
		outputFile opFile1,opFile2;


		float f;	//Probability of Friends
		float e;	//Probability of Enemies
		float i;	//Remaining Indifferent %
		int max_f_M;	// maximum number of friend relationships based on f
		int max_e_M;	// maximum number of enemie relationships based on e
		int max_i_M;	// maximum number indifferent relationships based on i
		

	public:
		int M;	//Number of Guests
		int N;	//Number of Tables
		Sentence sSentence;

	public:
		InstanceGenerator(int _M,int _N,int _f,int _e)
		{
			M = _M;
			N = _N;
			f = (float)_f/100;
			e = (float)_e/100;
			i = (float)1-f-e;
			max_f_M = (int)((M*(M-1))/2)*f;
			cout << "Friend Relationships possible: "<< max_f_M << endl;
			max_e_M = (int)((M*(M-1))/2)*e;
			cout << "Enemy Relationships possible: "<< max_e_M << endl;
			max_i_M = ((M*(M-1))/2) - max_f_M - max_e_M;
			cout << "Indifferent Relationships possible: "<< max_i_M << endl;

			opFile1.openFile("Clauses.txt");
			opFile2.openFile("RMatrix.txt");

			sSentence.clear();
		}

		void generateRMatrix()
		{
			int R[M][M];
			int f_count = max_f_M;
			int e_count = max_e_M;
			srand(time(0));
			sSentence.clear();
			
			//Setting initial value of -99 for Debugging.
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < M; ++j)
				{
					R[i][j] = -99;
				}
			}
			int friends =0,enemy =0;
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < M; ++j)
				{
					// Generate number from 1 to 100
					int k = rand() % 100 + 1; 

					//Modify values that has not been modified before.
					if(R[i][j] == -99)
					{
						if(i == j)
						{
							R[i][j] = 99;
						}					
						else if(k <= (f*100))
						{
							R[i][j] = R[j][i] = 1;
							friends++;
						}
						else if (k > (f*100) && k <= (e+f)*100)
						{
							R[i][j] = R[j][i] = -1;
							enemy++;
						}
						else
						{
							R[i][j] = 0;
						}
					}					
				}
			}
			//cout << "Friends : "<<friends<<" enemies: "<<enemy<<endl;

			//printRMatrix((int *)R);

			generateCNF((int *)R);
		}

		void generateCNF(int *_R)
		{
			int x = 1;

			//There are 4 constraints for which CNF sentences needs to be generated.
			/*
				1. Each guest can sit at atleast 1 table
				X11 v X12 v ... v X1N
				N => Number of Tables
			*/
			for (int i = 0; i < M; ++i)
			{
				Clause *cTemp = new Clause();

				for (int j = 0; j < N; ++j)
				{
					//Add the literal to the clause
					cTemp->push_back(Literal(i,j,false));
				}

				//Add the Clause to the sentence
				sSentence[x] = *cTemp;
				++x;

				delete cTemp;
			}

			/*
				2. Each guest can sit at atmost 1 table.
				X11 => ¬X12		¬X11 v ¬X12
				X11 => ¬X13		¬X11 v ¬X13
				...
				X1n => ¬X1j 	¬X1n v ¬X1j
				(where 1<= j != n <= N)
			*/
			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					for (int k = j+1; k < N; ++k)
					{
						Clause *cTemp = new Clause();

						//Add the literal to the clause
						cTemp->push_back(Literal(i,j,true));
						cTemp->push_back(Literal(i,k,true));
						Sentence::reverse_iterator ritS;
						ritS = sSentence.rbegin();
						x = ritS->first+1;
						sSentence[x] = *cTemp;

						delete cTemp;
					}
				}
			}

			/*
				3. Any 2 friends should be seated at the same table
				X11 <=> X21		(X11 => X21) ^ (X21 => X11)		(¬X11 v X21) ^ (¬X21 v X11)
				X1n <=> X2n		(X1n => X2n) ^ (X2n => X1n)		(¬X1n v X2n) ^ (¬X2n v X1n)
				Xij <=> Xkj (where k != i)
			*/
			for (int i = 0; i < M; ++i)
			{
				for (int j = i+1; j < M; ++j)
				{
					if(*(_R + i*M + j) == 1)
					{
						//i and j are friends
						//Generate the CNF clauses
						for (int k = 0; k < N; ++k)
						{
							//For each table create the friendship clauses for i and j
							Clause *cTemp1 = new Clause();

							//(¬X1n v X2n)
							cTemp1->push_back(Literal(i,k,true));
							cTemp1->push_back(Literal(j,k,false));
							Sentence::reverse_iterator ritS;
							ritS = sSentence.rbegin();
							x = ritS->first+1;
							sSentence[x] = *cTemp1;

							//(¬X2n v X1n)
							Clause *cTemp2 = new Clause();
							cTemp2->push_back(Literal(j,k,true));
							cTemp2->push_back(Literal(i,k,false));
							ritS = sSentence.rbegin();
							x = ritS->first+1;
							sSentence[x] = *cTemp2;

							delete cTemp1;
							delete cTemp2;
						}
					}
				}
			}
			/*
				4. Any 2 enemies should be seated at different table
				Xin => ¬Xjn  and Xjn => ¬Xin
				(¬Xin v ¬Xjn) ^ (¬Xjn v ¬Xin)
			*/
			for (int i = 0; i < M; ++i)
			{
				for (int j = i+1; j < M; ++j)
				{
					if (*(_R+ i*M + j) == -1)
					{
						//i and j are enemies
						//Generate the CNF clauses
						for (int k = 0; k < N; ++k)
						{
							//For each table create the enemy clause for i and j
							Clause *cTemp1 = new Clause();

							//(¬Xin v ¬Xjn)
							cTemp1->push_back(Literal(i,k,true));
							cTemp1->push_back(Literal(j,k,true));
							Sentence::reverse_iterator ritS;
							ritS = sSentence.rbegin();
							x = ritS->first+1;
							sSentence[x] = *cTemp1;
							/*
							//(¬Xjn v ¬Xin)
							Clause *cTemp2 = new Clause();

							cTemp2->push_back(Literal(j,k,true));
							cTemp2->push_back(Literal(i,k,true));
							ritS = sSentence.rbegin();
							x = ritS->first+1;
							sSentence[x] = *cTemp2;
							*/
							delete cTemp1;
							//delete cTemp2;
						}
					}
				}
			}
				//printSentence();				
		}

		void printSentence()
		{
			Sentence::reverse_iterator ritS;
			stringstream sPrintData;

			ritS = sSentence.rbegin();

			cout << "Total Number of initial Clauses generated: "<<ritS->first<<endl;
			sPrintData << ritS->first;
			str printData = "Total Number of Clauses generated: "+sPrintData.str();
			opFile1.writeToFile(printData);
			sPrintData.str("");

			//cout << "\nList of Clauses: "<<endl;
			opFile1.writeToFile("\nList of Clauses: \n");

			Sentence::iterator itS;
			for ( itS = sSentence.begin(); itS != sSentence.end(); ++itS)
			{
				//cout<<"Clause #: "<<itS->first<<" :: ";
				sPrintData << itS->first;
				printData = "Clause "+sPrintData.str()+" :: ";
				opFile1.writeToFile(printData);
				sPrintData.str("");
				Clause cc = itS->second;
				for (Clause::iterator i = cc.begin(); i != cc.end(); ++i)
				{
					sPrintData << ((*i).guest_number+1);
					sPrintData << ((*i).table_number+1);

					if((*i).isNegated == true)
					{
						//cout<<"-X"<<(*i).guest_number+1<<(*i).table_number+1<<" v ";
						printData = "-X"+sPrintData.str()+" ";
					}
					else
					{
						//cout<<"X"<<(*i).guest_number+1<<(*i).table_number+1<<" v ";
						printData = "X"+sPrintData.str()+" ";
					}
					opFile1.writeToFile(printData);
					sPrintData.str("");
				}
				//cout << endl;
				opFile1.writeToFile("\n");
			}
		}

		void printRMatrix(int *_R)
		{
			stringstream sPrintData;

			opFile2.writeToFile("1 => Friends,\n-1 => Enemies,\n0 => Indifferent,\n99 => No Relationship.\n\n");

			for (int i = 0; i < M; ++i)
			{
				for (int j = 0; j < M; ++j)
				{
					cout << *(_R + i*M + j)<<"\t";

					sPrintData << *(_R + i*M + j);

					str printData = sPrintData.str()+"\t";
					opFile2.writeToFile(printData);
					sPrintData.str("");
				}
				cout << endl;
				opFile2.writeToFile("\n");
			}
		}

		~InstanceGenerator()
		{
			opFile1.closeFile();
			opFile2.closeFile();
		}
};

//Get command line arguments and send it to function.
int main(int argc, char *argv[])
    {
    	//Generate an Instance of the relationship
    	   	WALK_SAT walkSAT;
    	   	outputFile outFile;
    	   	//InstanceGenerator IG;
    	   	PL_RESOLVE plResolver;
    	   	
    	   	char* cExp = argv[1];
    	   	str exp(cExp); 

    	   	if(exp == "exp1")
    	   	{
    	   		outFile.openFile("exp1.csv");
	    	   	//Exp 1
	    	   	for (int i = 2; i <= 20; i+=2)
	    	   	{
	    	   		int p =0,q=0;
	    	   			InstanceGenerator IG(16,2,0,i);
	    	   		for (int j = 1; j <= 100; ++j)
	    	   		{
	    	   			IG.generateRMatrix();
	    	   			if(plResolver.pl_resolution(IG.sSentence))
						{
							cout << "Resolution returned TRUE!\n";
							p++;
						}
						else
						{
							cout << "Resolution returned FALSE\n";
						}
	    	   			walkSAT.setWalkSATParam(IG.sSentence,50,100,IG.M,IG.N);
	    	   			if(walkSAT.doWalkSAT())
						{
							//cout <<j << "--WALK_SAT returned TRUE!\n";
							q++;
						}
						else{
							//cout <<j<< " --WALK_SAT returned FALSE!\n";
						}
	    	   		}
	    	   			stringstream ss;
		    	   	ss << i;
		    	   	str printData = ss.str()+",";
	    	   		ss.str("");
	    	   		ss << p;
	    	   		printData += ss.str()+",";
	    	   		ss.str("");
	    	   		cout << i <<" :p: "<<p<<endl;
	    	   		ss << q;
	    	   		printData += ss.str()+"\n";
	    	   		ss.str("");
	    	   		cout << i <<" :q: "<<q<<endl;
	    	   		outFile.writeToFile(printData);
	    	   	}
	    	   	outFile.closeFile();
    	   }
    	   else if(exp == "exp2")
    	   {
	    	   	//EXP 2 -Part 1
	    	   	outFile.openFile("exp2.csv");
	    	   	for (int i = 2; i <= 20; i=i+2)
	    	   	{
	    	   		int p = 0,q =0,r =0;
	    	   		cout << "i : "<<i<<endl;
	    	   		InstanceGenerator IG(16,2,i,5);
	    	   		for (int j = 1; j <= 100; ++j)
		    	   	{
		             	IG.generateRMatrix();
		             	walkSAT.setWalkSATParam(IG.sSentence,50,100,IG.M,IG.N);
		             	// cout<< "max_flips 100 Clause Count: "<<(IG.sSentence).size()<<endl;
		             	if(walkSAT.doWalkSAT())
						{
							//cout <<j << "--WALK_SAT returned TRUE!\n";
							p++;
						}
						walkSAT.setWalkSATParam(IG.sSentence,50,500,IG.M,IG.N);
						// cout<< "max_flips 500 Clause Count: "<<(IG.sSentence).size()<<endl;
		             	if(walkSAT.doWalkSAT())
						{
							//cout <<j<< "--WALK_SAT returned TRUE!\n";
							q++;
						}
						walkSAT.setWalkSATParam(IG.sSentence,50,1000,IG.M,IG.N);
						// cout<< "max_flips 1000 Clause Count: "<<(IG.sSentence).size()<<endl;
		             	if(walkSAT.doWalkSAT())
						{
							//cout << "WALK_SAT returned TRUE!\n";
							r++;
						}
		    	   	}
		    	   	stringstream ss;
		    	   	ss << i;
		    	   	str printData = ss.str()+",";
	    	   		ss.str("");
	    	   		ss << p;
	    	   		printData += ss.str()+",";
	    	   		ss.str("");
	    	   		cout << i <<" :p: "<<p<<endl;
	    	   		ss << q;
	    	   		printData += ss.str()+",";
	    	   		ss.str("");
	    	   		cout << i <<" :q: "<<q<<endl;
	    	   		ss << r;
	    	   		printData += ss.str()+"\n";
	    	   		ss.str("");
	    	   		cout << i <<" :r: "<<r<<endl;
	    	   		outFile.writeToFile(printData);
	    	   		ss.str("");
	    	   	}

	    	   	outFile.closeFile();
    	   }
    	   else if(exp == "exp3")
    	   {
    	   	float clauseCount = 0.0f;

    	   		outFile.openFile("exp3.csv");
    	   		int M=8,N=1;
    	   		for (int i = 0; i < 5; ++i)
    	   		{
    	   			M = M+8;
    	   			N = N+1;
    	   			int p=0;
    	   			InstanceGenerator IG(M,N,2,2);
    	   			while(p < 100)
    	   			{
    	   				IG.generateRMatrix();
    	   				walkSAT.setWalkSATParam(IG.sSentence,50,1000,IG.M,IG.N);
    	   				if (walkSAT.doWalkSAT())
    	   				{
    	   					clauseCount += (IG.sSentence).size();
    	   					p++;
    	   				}

    	   			}

	    	   		stringstream ss;
		   			cout <<" :M: "<<M<<endl;
		    	   	ss << M;
		    	   	str printData = ss.str()+",";
	    	   		ss.str("");

	    	   		cout <<" :N: "<<N<<endl;
	    	   		ss << N;
	    	   		printData += ss.str()+",";
	    	   		ss.str("");

	    	   		cout <<" :Iteration: "<<iteration/100.0<<endl;
	    	   		ss<<iteration/100.0;
	    	   		printData += ss.str()+",";
	    	   		iteration =0.0;
	    	   		ss.str("");

	    	   		//Sentence::reverse_iterator rIts;
	    	   		//rIts = (IG.sSentence).rbegin();
	    	   		cout << "Clauses: "<<clauseCount<< " Symbols: "<< M*N<<endl;
	    	   		cout << " :Clause/Literal Ratio: "<<clauseCount/(M*N*100)<<endl;
	    	   		ss<<(clauseCount/(M*N*100));
	    	   		clauseCount=0.0;
	    	   		printData += ss.str()+"\n";
	    	   		ss.str("");

	    	   		outFile.writeToFile(printData);

    	   		}
	   			
    	   		outFile.closeFile();
    	   }
   
    return EXIT_SUCCESS;
    }


