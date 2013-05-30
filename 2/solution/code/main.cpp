#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define FROM 0
#define TO 1
#define TIME 2
#define RISK 3
#define NAME 0
#define DIRECTTIME 1
#define DIRECTRISK 2
#define GREEDY 0
#define ASTAR 1

using namespace std;
typedef std::string str;

/*
  Input file Class
*/
class inputFile
{
public:
    ifstream    iFile;
    char* fileName;
public:
       inputFile(char* _fileName)
        {
                     fileName = _fileName;
        }
        
        void openFile()
        {
             iFile.open(fileName);
             }

    void closeFile()
        {
        iFile.close();
        }
};

/*
  Output file Class
*/
class   outputFile
{
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
   Connection class holds neighbour information for a specific person.
   The person information is avaialble in map (as a string), to which 
   each Connection object is associated. 
*/
class   Connection
{
public:
    str connectedTo;    // Neighbour
    int costTime;       // Time taken by the neighbour to read the message.
    int costRisk;       // Risk invloved in sending the message to the neighbour
public:
    Connection(str _person, int _time, int _risk)
        {
        connectedTo = _person;
        costTime = _time;
        costRisk = _risk;
        }
};

/* 
   ConnectionDirect class holds direct access 
   information for a specific person.
*/
class ConnectionDirect
{
public:
       int directTime;
       int directRisk;    
public:
       ConnectionDirect(int _time, int _risk)
       {
                directTime = _time;
                directRisk = _risk;         
       }
      
};

/*
  Break input and load it into the Graph
*/
class InputManipulator
{
      char* token;
      char    *writable;
 public:
        char* getToken(str _line)
        {
              if (_line != "")
              {
                     /*
                       Build a writable char * for splitting the input
                       REFERENCE Stackoverflow: http://stackoverflow.com/questions/347949/convert-stdstring-to-const-char-or-char
                     */
                     writable = new char[_line.size() + 1];
                     std::copy(_line.begin(), _line.end(), writable);
                     writable[_line.size()] = '\0';
                     
                     token = strtok(writable, " ");
                     return token;
              }
        }
        
        void getConnectionData(str (&_connectionData)[4],char* _token)
        {
                int i;
                /*
                  Generate and return connectionData Array
                */
                if(_token != NULL)
                {
                   _connectionData[0] = _token;
                   i = 1;       
                }
                
                while(_token != NULL)
                {
                        _token = strtok(NULL, " ");
                        if (_token != NULL && i < 4)
                            {
                                  _connectionData[i] = _token;
                                  i++;
                            }     
                }                
        }
        
        ~InputManipulator()
        {
            delete[] writable;                           
        }
};

/*
  Class that will build the graph from the input file.
  std::map is used to store the data internally.
  string -> will store the person name
  Connection object -> will store the neighbour information for that person
*/
class   socialNetworkGraph
{
        typedef vector<Connection> vConnect;
        typedef std::map<str, vConnect> gMap;
        
        public:
               gMap Graph;        
        private:
                str line,connectionData[4];                
                char *token;
                outputFile opGraph;
                InputManipulator iManip;
        public:
                int buildGraph(char* _fileNameNetwork)
                {
                     std::map<str, vConnect>::iterator pos;
                     inputFile ipFileSocialNetwork(_fileNameNetwork);
                     ipFileSocialNetwork.openFile();
                     
                     /*
                       Build the Social Network Graph
                       1. Read line from file and split into 4 parts
                       2. Populate connectionData 
                       3. Build the adjacency list for the graph.
                     */
                     
                     /*
                       Check if Input file is Open
                       else return error!
                     */
                     if (ipFileSocialNetwork.iFile.is_open())
                     {
                         while(ipFileSocialNetwork.iFile.good() && !ipFileSocialNetwork.iFile.eof())
                         {
                               //Read a line at a time and store it in "line"
                               getline(ipFileSocialNetwork.iFile, line);          
                               token = iManip.getToken(line);
                               iManip.getConnectionData(connectionData, token);
                               
                               /*
                                 We now have the connectionData array, we need to push it into the map.
                                 Each row of the map will contain the person name, and a pointer to a vector listing their neighbours.
                                 PASS 1 Alice -> Dan    
                               */   
                                
                               //Create a temp vector object that will hold the Connection objects.
                               vConnect *vTemp = new vConnect();
                               
                               //Check if the person is already in the graph
                               pos = Graph.find(connectionData[0]);
                               if (pos == Graph.end())
                               {
                                  //New Name, add to Graph
                                  vTemp->push_back(Connection(connectionData[1].c_str(),atoi(connectionData[2].c_str()),atoi(connectionData[3].c_str())));
                                  Graph[connectionData[0]] = *vTemp;
                               }
                               else
                               {
                                   (pos->second).push_back(Connection(connectionData[1].c_str(),atoi(connectionData[2].c_str()),atoi(connectionData[3].c_str())));
                               }
                               
                               /*
                                 PASS 2 Dan -> Alice
                               */
                               pos = Graph.find(connectionData[1]);
                               if (pos == Graph.end())
                               {
                                  //New Name, add to Graph
                                  vTemp->push_back(Connection(connectionData[0].c_str(),atoi(connectionData[2].c_str()),atoi(connectionData[3].c_str())));
                                  Graph[connectionData[1]] = *vTemp;
                               }
                               else
                               {
                                   (pos->second).push_back(Connection(connectionData[0].c_str(),atoi(connectionData[2].c_str()),atoi(connectionData[3].c_str())));
                               }
                         }
                         ipFileSocialNetwork.closeFile();
                         return EXIT_SUCCESS;                       
                     }
                     else
                     {
                         return EXIT_FAILURE;
                     }
                }
                
                void printGraph()
                {
                     opGraph.openFile("graph.txt");

                     std::map<str, vConnect>::iterator   pos = Graph.begin();
                     while (pos != Graph.end())
                     {
                           stringstream stime,srisk;
                   
                           //Access the state of the node Name [Key]
                           cout << "Name: " << pos->first << endl;

                           string toFile = "Name: " + pos->first + "\n";

                           //Access the vector [Value]
                           vConnect v = pos->second;
                           for (int x = 0; x < v.size(); x++)
                           {
                               Connection c = v[x];
                               cout << "\tFriend: " << c.connectedTo;
                               cout << " Time: " << c.costTime;
                               cout << " Risk: " << c.costRisk << endl;
                               stime << c.costTime;
                               srisk << c.costRisk;

                               toFile += "\tFriend: " + c.connectedTo+" Time: "+stime.str()+" Risk: "+srisk.str();
                               toFile += "\n";
                
                               stime.str("");
                               srisk.str("");
                           }

                           ++pos;
                           opGraph.writeToFile(toFile);
                      }

                      opGraph.closeFile();
                }
                
                void cleanGraph()
                {
                     std::map<str, vConnect>::iterator   pos = Graph.begin();
                     while (pos != Graph.end())
                     {
                           //Access the state of the node Name [Key]
                           if (pos->first == (pos->second)[0].connectedTo)
                           {
                              (pos->second).erase((pos->second).begin());
                           }

                           ++pos;
                    }
               }
};

/*
  Class that will build the graph from the input file.
  std::map is used to store the data internally.
  string -> will store the person name
  Connection object -> will store the neighbour information for that person
*/
class   DirectEstimate
{
        typedef vector<ConnectionDirect> dConnect;
        typedef std::map<str, dConnect> dMap;
        
        public:
               dMap estimateMap;        
        private:
                str line,connectionData[4];                
                char *token;
                InputManipulator iManip;
                outputFile opFileGraphEstimate;
        public:
                int buildDirectEstimate(char* _fileNameEstimate)
                {
                     std::map<str, dConnect>::iterator pos;
                     inputFile ipFileEstimate(_fileNameEstimate);
                     ipFileEstimate.openFile();
                     
                     /*
                       Check if Input file is Open
                       else return error!
                     */
                     if (ipFileEstimate.iFile.is_open())
                     {
                         while(ipFileEstimate.iFile.good() && !ipFileEstimate.iFile.eof())
                         {
                               //Read a line at a time and store it in "line"
                               getline(ipFileEstimate.iFile, line);          
                               token = iManip.getToken(line);
                               iManip.getConnectionData(connectionData, token);
                               
                               /*
                                 We now have the connectionData array, we need to push it into the map.
                                 Each row of the map will contain the person name, and a pointer to a vector listing their direct cost.   
                               */   
                                
                               //Create a temp vector object that will hold the Connection objects.
                               dConnect *vTemp = new dConnect();
                               
                                  //New Name, add to Graph
                                  vTemp->push_back(ConnectionDirect(atoi(connectionData[1].c_str()),atoi(connectionData[2].c_str())));
                                  estimateMap[connectionData[0]] = *vTemp;

                         }
                         ipFileEstimate.closeFile();
                         
                         return EXIT_SUCCESS;                       
                     }
                     else
                     {
                         return EXIT_FAILURE;
                     }
                }
                
                void setDirectCostforRecipient(str _recipient)
                {
                          //Insert direct time data for Recipient
                         dConnect *vTemp = new dConnect();
                         
                         //The Cost of reaching the recipient from recipient is 0
                         vTemp->push_back(ConnectionDirect(0,0));
                         estimateMap[_recipient] = *vTemp; 
                }
                
                void printDirectEstimate()
                {
                     opFileGraphEstimate.openFile("estimate.txt");

                     std::map<str, dConnect>::iterator pos = estimateMap.begin();
                     while (pos != estimateMap.end())
                     {
                           stringstream stime,srisk;
                   
                           //Access the state of the node Name [Key]
                           cout << "Name: " << pos->first;

                           string toFile = "Name: " + pos->first;

                           //Access the vector [Value]
                           dConnect v = pos->second;
                           for (int x = 0; x < v.size(); x++)
                           {
                               ConnectionDirect c = v[x];
                               cout << " Time: " << c.directTime;
                               cout << " Risk: " << c.directRisk << endl;
                               stime << c.directTime;
                               srisk << c.directRisk;

                               toFile += " Time: "+stime.str()+" Risk: "+srisk.str();
                               toFile += "\n";
                
                               stime.str("");
                               srisk.str("");
                           }

                           ++pos;
                           opFileGraphEstimate.writeToFile(toFile);
                      }

                      opFileGraphEstimate.closeFile();
                }                
}; 

class Search
{
      typedef vector<Connection> vConnect;
      typedef std::map<str, vConnect> gMap; 
      typedef vector<ConnectionDirect> dConnect;
      typedef std::map<str, dConnect> dMap;
      
      typedef std::list<str> myList;
      typedef vector<str> vec;
      /*
        Node - This class will contain the node information in the tree.
             State - Name of the node
             Depth - Level of the node
             Parent - Parent of the node
             nodeNumber -Ordinal number of the node
             Cost - Cost to reach/spent in reaching the node from previous node
             hOfN - Direct cost to reach the goal from current node (Greedy)
      */
      class Node
      {
            public:
                   str State;
                   int Depth, Parent,nodeNumber;
                   int Cost,hOfN;
            public:
                   Node (int _nodeNumber, str _state, int _depth, int _cost, int _parent,int _hOfN)
                   {
                        nodeNumber = _nodeNumber;
                        State = _state;
                        Depth = _depth;
                        Cost = _cost;
                        Parent = _parent;
                        hOfN = _hOfN;
                   }
                   /*
                     Overloaded < Operator used to maintain the priority Q based on Cost
                   */
                   friend bool operator < (const Node &_n1, const Node &_n2)
                   {
                          return _n1.hOfN < _n2.hOfN;
                   }
      };

      /*
        Node - This class will contain the node information in the tree.
             State - Name of the node
             Depth - Level of the node
             Parent - Parent of the node
             nodeNumber -Ordinal number of the node
             Cost - Total Cost to reach/spent in reaching the node from start.
             hOfN - Direct cost to reach the goal from current node
      */
      class AStarNode
      {
            public:
                   str State;
                   int Depth, Parent,nodeNumber;
                   int Cost,hOfN,fOfN;
            public:
                   AStarNode (int _nodeNumber, str _state, int _depth, int _cost, int _parent,int _hOfN)
                   {
                        nodeNumber = _nodeNumber;
                        State = _state;
                        Depth = _depth;
                        Cost = _cost; //g(n)
                        Parent = _parent;
                        hOfN = _hOfN;
                        fOfN = Cost + hOfN; //f(n) = g(n)+h(n)
                   }
                   /*
                     Overloaded < Operator used to maintain the priority Q based on Cost
                   */
                   friend bool operator < (const AStarNode &_n1, const AStarNode &_n2)
                   {
                          return _n1.fOfN < _n2.fOfN;
                   }
      };

      private:
            /*
              Black -> List of unvisited nodes
              Grey -> List of visied nodes
            */
            list<Node> black, grey;
            list<Node>::iterator itBlack,itGrey;

            list<AStarNode> aStarBlack, aStarGrey;
            list<AStarNode>::iterator itAStarBlack,itAStarGrey;
            
            socialNetworkGraph *snGraph;
            DirectEstimate *deMap;
            
            gMap::iterator gPos;
            dMap::iterator dPos;
            
            str Sender, Recipient, Path;
            myList expandList, visitedList;
            
            multimap<string, string> childParentMap;
            multimap<string, string>::iterator itChildParentMap;
            
            outputFile searchGraph;
            
            int nodeNum;

      public:
        //Class Constructor
          Search(socialNetworkGraph *_snGraph,DirectEstimate *_deMap)
           {
                 snGraph = _snGraph;
                 deMap = _deMap;
                 nodeNum = 1;
           }

           //Common Functions
           void setRecipient(str _recipient)
           {
                Recipient = _recipient;
                
                deMap->setDirectCostforRecipient(Recipient);
           }
           
           void setSender(str _sender)
           {
                Sender = _sender;
                /*
                  We need to expand the first node i.e sender,
                  so add sender to the expandList
                */
                expandList.push_back(Sender);
           }

          void setSearchParameters()
           {
                //Set the Sender as Start node in Child Parent Map
                childParentMap.insert(pair<string, string> (Sender, "start"));
           }

           void setSenderPos()
           {
              //Get the iterator position of sender in graph
              gPos = snGraph->Graph.find(Sender);
              dPos = deMap->estimateMap.find(gPos->first);
           }

           bool testGoal(str _node)
           {
                return (Recipient == _node);
           }

           void printPath(str child, int _param)
            {
                  switch(_param)
                  {
                    case GREEDY:
                      //The Goal node will be present at the top of _black list
                      //Verify if child is equal to the goal node.
                      if (child == (black.front()).State)
                         {
                            //The Goal has been reached.
                            Path = child;
                            printParentOfNode(black.front());
                          }
                      break;
                    case ASTAR:
                      if(child == (aStarBlack.front()).State)
                        {
                            Path = child;
                            printParentOfAStarNode(aStarBlack.front());         
                        }
                }
            }

            int getDirectCost(str _name,int _param)
             {
              dPos = deMap->estimateMap.find(_name);
              if(_param == TIME)
              {
                return (dPos->second)[0].directTime;
              }
              else if (_param == RISK)
              {
                return (dPos->second)[0].directRisk;
              }
             }

          template <class T>
          bool removeNodeFromBlack(T _n, str _state,int _param)
          {
            switch(_param)
            {
              case GREEDY:
                for (itBlack = black.begin(); itBlack != black.end(); itBlack++)
                  {
                  if ((*itBlack).State == _state)
                      {
                      black.erase(itBlack);
                      return true;
                      }
                  }
              case ASTAR:
                for (itAStarBlack = aStarBlack.begin(); itAStarBlack != aStarBlack.end(); itAStarBlack++)
                  {
                  if ((*itAStarBlack).State == _state)
                      {
                      aStarBlack.erase(itAStarBlack);
                      return true;
                      }
                  }
            }
            return false;
          }

           bool checkNodeinGrey(str _name, int _param)
           {
                switch(_param)
                {
                  case GREEDY:
                    //Iterate through the list and find the position of node in grey.
                    for (itGrey = grey.begin(); itGrey != grey.end() ; ++itGrey)
                    {
                      if ((*itGrey).State == _name)
                      {
                        return true;
                      }
                    }
                    break;
                  case ASTAR:
                    //Iterate through the list and find the position of node in grey.
                    for (itAStarGrey = aStarGrey.begin(); itAStarGrey != aStarGrey.end() ; ++itAStarGrey)
                    {
                      if ((*itAStarGrey).State == _name)
                      {
                        return true;
                      }
                    } 
                    break; 
                  default:
                    return false;            
                }
            return false;
           }

           bool checkNodeinBlack(str _name,int _param)
           {
              switch(_param)
              {
                case GREEDY:
                  //Iterate through the list and find the position of node in black.
                  for (itBlack = black.begin(); itBlack != black.end() ; ++itBlack)
                  {
                    if ((*itBlack).State == _name)
                    {
                      return true;
                    }
                  }
                  break;
                case ASTAR:
                  //Iterate through the list and find the position of node in aStarBlack.
                  for (itAStarBlack = aStarBlack.begin(); itAStarBlack != aStarBlack.end() ; ++itAStarBlack)
                  {
                    if ((*itAStarBlack).State == _name)
                    {
                      return true;
                    }
                  }
                  break; 
                default:
                  return false;               
              }
              return false;            
            }

           //FUNCTIONS FOR GREEDY
           str getParentState(int _parentNumber)
           {
               for (itGrey = grey.begin(); itGrey != grey.end(); itGrey++)
               {
                   if ((*itGrey).nodeNumber == _parentNumber)
                   {
                      return (*itGrey).State;
                   }
               }
               return "none";
           }

           void printParentOfNode(Node _node)
           {
                str parentState = getParentState(_node.Parent);

                if (_node.Parent != 0)
                {
                   Path = parentState + "-" + Path;

                   //Get the parent node in the list
                   for (itGrey = grey.begin(); itGrey != grey.end(); itGrey++)
                   {
                       if ((*itGrey).nodeNumber == _node.Parent)
                       {
                          break;
                       }
                   }
                   printParentOfNode((*itGrey));
                }
           }

          Node getNodeinBlack(str _name)
          {
                //Get the parent node in the list
                for (itBlack = black.begin(); itBlack != black.end(); itBlack++)
                  {
                    if ((*itBlack).State == _name)
                      {
                      return (*itBlack);
                      }
                  }
          }

          int searchGreedy(int _param)
           {
            if(!black.empty())
            {
              //Get the first element from the black Q
              gPos = snGraph->Graph.find((black.front()).State);

              //Check if Goal has been reached
              if(!testGoal(gPos->first))
              {
                //The goal has not been reached. 
                //Start expanding the current node.
                //Push the current node state into Grey
                grey.push_back(black.front());

                //Find the friends of Current Node
                for (int i = 0; i < (gPos->second).size(); ++i)
                {
                  //Check if the node is in grey
                  if (!checkNodeinGrey((gPos->second)[i].connectedTo,GREEDY))
                  {
                    //Check if the node is already present in black
                    if (!checkNodeinBlack((gPos->second)[i].connectedTo,GREEDY))
                    {
                      //This is a new node, so push into black for expanding.
                      nodeNum++;

                      /*
                        NodeNumber, State, Depth, Cost, Parent, hOfN
                      */
                        //Get the direct cost for the current node
                        int directCost = getDirectCost((gPos->second)[i].connectedTo,_param);
                        switch(_param)
                        {
                          case TIME:
                            black.push_back(Node(nodeNum,(gPos->second)[i].connectedTo,((black.front()).Depth)++,(gPos->second)[i].costTime,(black.front()).nodeNumber,directCost));
                            break;
                          case RISK:
                            black.push_back(Node(nodeNum,(gPos->second)[i].connectedTo,((black.front()).Depth)++,(gPos->second)[i].costRisk,(black.front()).nodeNumber,directCost));
                            break;
                          default:
                            return EXIT_FAILURE;
                        }
                        
                    }
                    else
                    {
                      //The node is already in black so compare new direct Cost and push the least into the Q
                      //Get the node present int the graph
                      Node n = getNodeinBlack((gPos->second)[i].connectedTo);
                      
                      int currentDirectCost = getDirectCost((gPos->second)[i].connectedTo,_param);

                      //Compare the direct cost of current and previous node
                      if (currentDirectCost < n.hOfN)
                      {
                        nodeNum++;

                        //Remove the previous node and push current node in black.
                        if (removeNodeFromBlack<Node>(n,n.State,GREEDY))
                        {
                          int directCost = getDirectCost((gPos->second)[i].connectedTo,_param);
                          switch(_param)
                          {
                            case TIME:
                              black.push_back(Node(nodeNum,(gPos->second)[i].connectedTo,((black.front()).Depth)++,(gPos->second)[i].costTime,(black.front()).nodeNumber,directCost));
                              break;
                            case RISK:
                              black.push_back(Node(nodeNum,(gPos->second)[i].connectedTo,((black.front()).Depth)++,(gPos->second)[i].costRisk,(black.front()).nodeNumber,directCost));
                              break;
                            default:
                              return EXIT_FAILURE;
                          }
                        }
                      }
                    }
                  }
                }

              //Remove from black and color grey
              black.pop_front();

              //sort thr lists based on cost
              black.sort();
              grey.sort();

              searchGreedy(_param);
            }
            else
            {
              printPath(gPos->first,GREEDY);

              switch(_param)
              {
                case TIME:
                  searchGraph.openFile("Greedy.time.result.txt");
                  break;
                case RISK:
                  searchGraph.openFile("Greedy.risk.result.txt");
                  break;
                default:
                  return EXIT_FAILURE;
              }  
              //cout<<Path;            
              searchGraph.writeToFile(Path);
              searchGraph.closeFile();
            }

           }
           else
           {
            Path = "Sender/Recipient could not be found!";
              searchGraph.openFile("Greedy.time.result.txt");
              searchGraph.writeToFile(Path);
              searchGraph.closeFile();            
           }
         }

         int startGreedy(int _param)
           {
              //Clear all lists and priority queues before searching
              expandList.clear();
              visitedList.clear();
              black.clear();
              grey.clear();
              Path="";

              setSender(Sender);
              setSenderPos();

              switch(_param)
              {
                case TIME:
                  //Push the Sender into Black Q. hofN = Direct Time
                  black.push_back(Node(1,gPos->first,0,0,0,(dPos->second)[0].directTime));
                  break;
                case RISK:
                  //Push the Sender into Black Q. hOfN = Direct Risk
                  black.push_back(Node(1,gPos->first,0,0,0,(dPos->second)[0].directRisk));
                  break;
                default:
                  //incorrect search parameter. Exit
                  return EXIT_FAILURE;
              }
              nodeNum = 1;  
              searchGreedy(_param);           
           }

        //Function for A*
           str getParentStateAStar(int _parentNumber)
           {
               for (itAStarGrey = aStarGrey.begin(); itAStarGrey != aStarGrey.end(); itAStarGrey++)
               {
                   if ((*itAStarGrey).nodeNumber == _parentNumber)
                   {
                      return (*itAStarGrey).State;
                   }
               }
               return "none";
           }

           void printParentOfAStarNode(AStarNode _node)
           {
                str parentState = getParentStateAStar(_node.Parent);

                if (_node.Parent != 0)
                {
                   Path = parentState + "-" + Path;

                   //Get the parent node in the list
                   for (itAStarGrey = aStarGrey.begin(); itAStarGrey != aStarGrey.end(); itAStarGrey++)
                   {
                       if ((*itAStarGrey).nodeNumber == _node.Parent)
                       {
                          break;
                       }
                   }
                   printParentOfAStarNode((*itAStarGrey));
                }
           }

           AStarNode getNodeinAStarBlack(str _name)
          {
                //Get the parent node in the list
                for (itAStarBlack = aStarBlack.begin(); itAStarBlack != aStarBlack.end(); itAStarBlack++)
                  {
                    if ((*itAStarBlack).State == _name)
                      {
                      return (*itAStarBlack);
                      }
                  }          
          }

          int searchAStar(int _param)
           {
            //check if the aStarBlack list is NOT empty
            if (!aStarBlack.empty())
            {
              //Get the first element from the aStarBlack List
              gPos = snGraph->Graph.find((aStarBlack.front()).State);

              //Check if Goal has NOT been reached
              if (!testGoal(gPos->first))
              {
                //The goal has not been reached,
                //start expanding the current node.
                //Push the current node state into aStarGrey
                aStarGrey.push_back(aStarBlack.front());

                //Find friends of current node
                for (int i = 0; i < (gPos->second).size(); ++i)
                {
                  //check if node is in aStarGrey
                  if (!checkNodeinGrey((gPos->second)[i].connectedTo,ASTAR))
                  {
                    if (!checkNodeinBlack((gPos->second)[i].connectedTo,ASTAR))
                    {
                      //This is a new node, push into aStarBlack
                      nodeNum++;

                      /*
                        NodeNumber, State, Depth, Cost, Parent, hOfN
                      */
                      //Get the direct cost for the current node
                      int directCost = getDirectCost((gPos->second)[i].connectedTo,_param);
                      switch(_param)
                      {
                        case TIME:                      
                          aStarBlack.push_back(AStarNode(nodeNum,(gPos->second)[i].connectedTo,((aStarBlack.front()).Depth)++,((gPos->second)[i].costTime + (aStarBlack.front()).Cost),(aStarBlack.front()).nodeNumber,directCost));
                          break;
                        case RISK:
                          aStarBlack.push_back(AStarNode(nodeNum,(gPos->second)[i].connectedTo,((aStarBlack.front()).Depth)++,((gPos->second)[i].costRisk + (aStarBlack.front()).Cost),(aStarBlack.front()).nodeNumber,directCost));
                          break;
                        default:
                          return EXIT_FAILURE;
                      }
                    }
                    else
                    {
                      //The node is already in aStarBlack so compare new direct Cost and push the least into the Q
                      //Get the node present int the graph
                      AStarNode n = getNodeinAStarBlack((gPos->second)[i].connectedTo);
                      
                      //INCORRECT NEED TO GET g(n)!!!!
                      int directCost = getDirectCost((gPos->second)[i].connectedTo,_param);
                      int currentFoFN = ((gPos->second)[i].costTime + (aStarBlack.front()).Cost)+directCost;

                      //Compare the direct cost of current and previous node
                      if (currentFoFN < n.fOfN)
                      {
                        nodeNum++;

                        //Remove the previous node and push current node in black.
                        if (removeNodeFromBlack<AStarNode>(n,n.State,ASTAR))
                        {
                          switch(_param)
                          {
                            case TIME:
                                 aStarBlack.push_back(AStarNode(nodeNum,(gPos->second)[i].connectedTo,((aStarBlack.front()).Depth)++,((gPos->second)[i].costTime + (aStarBlack.front()).Cost),(aStarBlack.front()).nodeNumber,directCost));
                                 break;
                            case RISK:
                                 aStarBlack.push_back(AStarNode(nodeNum,(gPos->second)[i].connectedTo,((aStarBlack.front()).Depth)++,((gPos->second)[i].costRisk + (aStarBlack.front()).Cost),(aStarBlack.front()).nodeNumber,directCost));
                                 break;
                            default:
                              return EXIT_FAILURE;
                          }
                        }
                      }
                    }
                  }
                }
                //Remove from black and color grey
                aStarBlack.pop_front();

                //sort thr lists based on cost
                aStarBlack.sort();
                aStarGrey.sort();

                searchAStar(_param);
              }
              else
              {
                printPath(gPos->first,ASTAR);

                switch(_param)
                {
                  case TIME:
                    searchGraph.openFile("A-star.time.result.txt");
                    break;
                  case RISK:
                    searchGraph.openFile("A-star.risk.result.txt");
                    break;
                  default:
                    return EXIT_FAILURE;
                }              
                searchGraph.writeToFile(Path);
                searchGraph.closeFile();
              }
            }
           } 

           int startAStar(int _param)
           {
              //Clear all lists and priority queues before searching
              expandList.clear();
              visitedList.clear();
              aStarBlack.clear();
              aStarGrey.clear();
              Path="";

              setSender(Sender);
              setSenderPos();

              switch(_param)
              {
                case TIME:
                  //Push the Sender into Black Q. hofN = Direct Time
                  aStarBlack.push_back(AStarNode(1,gPos->first,0,0,0,(dPos->second)[0].directTime));
                  break;
                case RISK:
                  //Push the Sender into Black Q. hOfN = Direct Risk
                  aStarBlack.push_back(AStarNode(1,gPos->first,0,0,0,(dPos->second)[0].directRisk));
                  break;
                default:
                  //incorrect search parameter. Exit
                  return EXIT_FAILURE;
              }

              nodeNum = 1;  

              searchAStar(_param);                
           }
};

//Get command line arguments and send it to function.
int main(int argc, char *argv[])
    {
             char* networkGrahpFile = argv[1];
             char* estimateCostFile = argv[2];
             
             const char* cSender = argv[3];
             const char* cRecipient = argv[4];
             
             //Convert from char to string
             //string (const char* s);
             str sender(cSender), recipient(cRecipient);             
             
    //Create Objects for Social Network Graph, Direct Estimate and Search Classes.
    socialNetworkGraph snGraph;
    DirectEstimate dEstimate;
    Search informedSearch(&snGraph,&dEstimate);
    
    //Build the graph i.e generate the adjacency list.
    snGraph.buildGraph(networkGrahpFile);
    snGraph.cleanGraph();
    
    //$DEBUG$
    //snGraph.printGraph ();
    //#DEBUG#
    
    //Build the Direct Estimate MAP
    dEstimate.buildDirectEstimate(estimateCostFile);
    
    //$DEBUG$
    //dEstimate.printDirectEstimate();
    //#DEBUG#
    
    //Set the Sender and Receiver data for initiating the search.
    informedSearch.setSender(sender);
    informedSearch.setRecipient(recipient);
    
    //Start the Searches
    informedSearch.setSearchParameters();
    informedSearch.startGreedy(TIME);
    informedSearch.startGreedy(RISK);
    informedSearch.startAStar(TIME);
    informedSearch.startAStar(RISK);
    
    return EXIT_SUCCESS;
    }


