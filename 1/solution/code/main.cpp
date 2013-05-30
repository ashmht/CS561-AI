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
#define EXIT_SUCCESS    0

using namespace     std;
typedef std::string str;

//Input file Class
class               inputFile
{
public:
    ifstream    iFile;
public:
    void openFile()
        {
        iFile.open("social-network.txt");
        }

    void closeFile()
        {
        iFile.close();
        }
};

//Output file Class
class   outputFile
{
    ofstream    oFile;
public:
    void openFile(str fileName)
        {
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
    int costRisk;       // Risk invloved insending the message to the neighbour
public:
    Connection(str person, int time, int risk)
        {
        connectedTo = person;
        costTime = time;
        costRisk = risk;
        }
};

//Class that will build the graph from the input file.

//std::map is used to store the data internally.

//string -> will store the person name

//Connection object -> will store the neighbour information for that person
class   classGraph
{
    typedef vector<Connection>      vConnect;
    typedef std::map<str, vConnect> gMap;
public:
    gMap Graph;
private:
    inputFile ipFile;
    str line;
    char *token;
    string connectionData[4];
    outputFile opGraph;
public:
    void buildGraph()
        {
        std::map<str, vConnect>::iterator   pos;
        int i;

        //Open the input file
        ipFile.openFile();

        //Build the Graph
        if (ipFile.iFile.is_open())
            {
            while (ipFile.iFile.good() && !ipFile.iFile.eof())
                {
                //Read one line at a time and store it in "line"
                getline(ipFile.iFile, line);

                if (line != "")
                    {
                    //Build a writable char * for splitting the input
                    //REFERENCE Stackoverflow: http://stackoverflow.com/questions/347949/convert-stdstring-to-const-char-or-char
                    char    *writable = new char[line.size() + 1];
                    std::copy(line.begin(), line.end(), writable);
                    writable[line.size()] = '\0';               //termination character

                    //Generate the connectionData Array
                    //Condition required in case of empty string in input.
                    token = strtok(writable, " ");
                    if (token != NULL)
                        {
                        connectionData[0] = token;
                        i = 1;
                        }

                    while (token != NULL)
                        {
                        token = strtok(NULL, " ");
                        if (token != NULL && i < 4)
                            {
                            connectionData[i] = token;
                            i++;
                            }
                        }

                    //Now we have the connection array, we need to push it into the map.
                    //Each row of the map will contain the person name, and a pointer to a vector listing their neighbours.
                    //PASS 1 Alice -> Dan
                    //Create the vector object that will hold the Connection objects.
                    vConnect    *vTemp = new vConnect();

                    pos = Graph.find(connectionData[0]);

                    if (pos == Graph.end())
                        {
                        //This name is not avaialable in the Map. So add it.
                        vTemp->push_back(Connection(connectionData[1].c_str(),
                                         atoi(connectionData[2].c_str()),
                                         atoi(connectionData[3].c_str())));
                        Graph[connectionData[0]] = *vTemp;
                        }
                    else
                        {
                        (pos->second).push_back(Connection(
                                                    connectionData[1].c_str(),
                                                atoi(connectionData[2].c_str()),
                                                        atoi(connectionData[3].c_str())
                                                                ));
                        }

                    //PASS 2 Dan -> Alice
                    pos = Graph.find(connectionData[1]);

                    if (pos == Graph.end())
                        {
                        //This name is not avaialable in the Map. So add it.
                        vTemp->push_back(Connection(connectionData[0].c_str(),
                                         atoi(connectionData[2].c_str()),
                                         atoi(connectionData[3].c_str())));
                        Graph[connectionData[1]] = *vTemp;
                        }
                    else
                        {
                        (pos->second).push_back(Connection(
                                                    connectionData[0].c_str(),
                                                atoi(connectionData[2].c_str()),
                                                        atoi(connectionData[3].c_str())
                                                                ));
                        }

                    //free the string after finished using it
                    delete[] writable;
                    }
                }
            }

        //Close the input file
        ipFile.closeFile();
        }

    void printGraph()
        {
        opGraph.openFile("graph.txt");

        std::map<str, vConnect>::iterator   pos = Graph.begin();
        while (pos != Graph.end())
            {
                   stringstream stime,srisk;

            //Access the state of the node Name [Key]
            //cout << "Name: " << pos->first << endl;

            string toFile = "Name: " + pos->first + "\n";

            //Access the vector [Value]
            vConnect v = pos->second;
            for (int x = 0; x < v.size(); x++)
                {
                Connection c = v[x];
                //cout << "\tFriend: " << c.connectedTo;
                //cout << " Time: " << c.costTime;
                //cout << " Risk: " << c.costRisk << endl;
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

class Search
{
    typedef vector<Connection> vConnect;
    typedef std::map<str, vConnect> gMap;
    typedef std::list<str> myList;
    typedef vector<str> vec;

    class Node
    {
    public:
        str State;
        int Depth, Parent,nodeNumber;
        int Cost;
    public:
        Node (int _nodeNumber, str _state, int _depth, int _cost, int _parent)
            {
            nodeNumber = _nodeNumber;
            State = _state;
            Depth = _depth;
            Cost = _cost;
            Parent = _parent;
            }
        friend bool operator < (const Node &n1, const Node &n2) //friend claim has to be here
            {
            return n1.Cost < n2.Cost;
            }
    };

    list<Node> _black, _grey;
    list<Node>::iterator iBlack,
                        iGrey;
    classGraph *G;
    str Sender, Recipient;
    myList expandList, visitedList;
    gMap::iterator pos;
    str Path;
    multimap<string, string> cpMap;
    multimap<string, string>::iterator iCPMap;
    outputFile bfsGraph;
    int _nodeNum;
public:
    Search(classGraph *graph)
        {
        G = graph;
        _nodeNum = 1;
        }

    bool testGoal(str Node)
        {
        if (Recipient == Node)
            {
            return true;
            }
        else
            {
            return false;
            }
        }

    bool isVisited(string _type)
        {
        myList::iterator    pos;

        if (_type == "search")
            {
            pos = find(visitedList.begin(), visitedList.end(),
                       expandList.front());
            }

        if (pos == visitedList.end())
            {
            return false;
            }
        else
            {
            return true;
            }
        }

    void setRecipient(str _recipient)
        {
        Recipient = _recipient;
        }

    void setSender(str _sender)
        {
        Sender = _sender;

        //Add sender to the expandList
        expandList.push_back(Sender);
        }

    str getParentState(int _parentNumber)
        {
        for (iGrey = _grey.begin(); iGrey != _grey.end(); iGrey++)
            {
            if ((*iGrey).nodeNumber == _parentNumber)
                {
                return (*iGrey).State;
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
            for (iGrey = _grey.begin(); iGrey != _grey.end(); iGrey++)
                {
                if ((*iGrey).nodeNumber == _node.Parent)
                    {
                    break;
                    }
                }

            printParentOfNode((*iGrey));
            }
        }

    void printPath(str child, string _searchType)
        {
        if (_searchType == "search")
            {
            iCPMap = cpMap.find(child);

            if (iCPMap->second != "start")
                {
                Path = iCPMap->second + "-" + Path;
                child = iCPMap->second;
                printPath(child, "search");
                }
            }
        else if (_searchType == "sort")
            {
            //The Goal node will be present at the top of _black list
            //Verify if child is equal to the goal node.
            if (child == (_black.front()).State)
                {
                //The Goal has been reached.
                Path = child;
                printParentOfNode(_black.front());
                }
            }
        }

    void startSearches()
        {
        //Get the position of the sender in gMap
        pos = G->Graph.find(Sender);

        cpMap.insert(pair<string, string> (Sender, "start"));

        //BFS
        expandBFS();
        expandList.clear();
        visitedList.clear();
        Path = "";

        //DFS
        setSender(Sender);
        pos = G->Graph.find(Sender);
        expandDFS();
        expandList.clear();
        visitedList.clear();
        Path = "";

        //Uniform-Cost: Time
        setSender(Sender);
        pos = G->Graph.find(Sender);
        expandList.clear();
        visitedList.clear();
        _black.push_back(Node(1, pos->first, 0, 0, 0));
        _nodeNum = 1;
        expandUCT();
        _black.clear();
        _grey.clear();
        Path = "";

        //Uniform-Cost: Risk
        setSender(Sender);
        pos = G->Graph.find(Sender);
        expandList.clear();
        visitedList.clear();
        _black.push_back(Node(1, pos->first, 0, 0, 0));
        _nodeNum = 1;
        expandUCR();
        Path = "";
        }

    void expandBFS()
        {
        bool goalStatus;

        while (isVisited("search"))
            {
            expandList.pop_front();
            }

        if (!expandList.empty())
            {
            //Check if goal has been reached
            pos = G->Graph.find(expandList.front());
            goalStatus = testGoal(pos->first);
            if (goalStatus == false)
                {
                //expand node
                //push it into visitedList
                visitedList.push_back(expandList.front());

                //Find friends of Node
                for (int i = 0; i < (pos->second).size(); i++)
                    {
                    expandList.push_back((pos->second)[i].connectedTo);

                    //Enter child parent values
                    cpMap.insert(pair<string, string> ((pos->second)[i].connectedTo, expandList.front()));
                    }

                //remove from current list
                expandList.pop_front();

                expandBFS();
                }
            else
                {
                Path = pos->first;
                //cout << "BFS Path : ";
                printPath(pos->first, "search");
                //cout << Path;
                bfsGraph.openFile("breadth-first.result.txt");
                bfsGraph.writeToFile(Path);
                bfsGraph.closeFile();
                }
            }
        else
            {
            Path = "Recipient could not be found!";
            cout << Path;
            bfsGraph.openFile("breadth-first.result.txt");
            bfsGraph.writeToFile(Path);
            bfsGraph.closeFile();
            }
        }

    void expandDFS()
        {
        bool goalStatus;
        while (isVisited("search"))
            {
            expandList.pop_front();
            }

        if (!expandList.empty())
            {
            //Check if goal has been reached
            pos = G->Graph.find(expandList.front());

            goalStatus = testGoal(pos->first);
            if (goalStatus == false)
                {
                //expand node
                //push it into visitedList
                visitedList.push_front(expandList.front());

                Path += expandList.front() + "-";

                //Find friends of Node
                for (int i = 0; i < (pos->second).size(); i++)
                    {
                    expandList.push_front((pos->second)[i].connectedTo);
                    }

                expandDFS();
                }
            else
                {
                //cout << "\nDFS path : ";
                Path += pos->first;
                //cout << Path;
                bfsGraph.openFile("depth-first.result.txt");
                bfsGraph.writeToFile(Path);
                bfsGraph.closeFile();
                }
            }
        else
            {
            Path = "Recipient could not be found!";
            //cout << Path;
            bfsGraph.openFile("depth-first.result.txt");
            bfsGraph.writeToFile(Path);
            bfsGraph.closeFile();
            }
        }

    bool checkNodeinGrey(str _name)
        {
        //Get the parent node in the list
        for (iGrey = _grey.begin(); iGrey != _grey.end(); iGrey++)
            {
            if ((*iGrey).State == _name)
                {
                return true;
                }
            }

        return false;
        }

    bool checkNodeinBlack(str _name)
        {
        //Get the parent node in the list
        for (iBlack = _black.begin(); iBlack != _black.end(); iBlack++)
            {
            if ((*iBlack).State == _name)
                {
                return true;
                }
            }

        return false;
        }

    Node getNodeinBlack(str _name)
        {
        //Get the parent node in the list
        for (iBlack = _black.begin(); iBlack != _black.end(); iBlack++)
            {
            if ((*iBlack).State == _name)
                {
                return (*iBlack);
                }
            }
        }

    bool removeNodeFromBlack(Node _n, str _state)
        {
        for (iBlack = _black.begin(); iBlack != _black.end(); iBlack++)
            {
            if ((*iBlack).State == _state)
                {
                _black.erase(iBlack);
                return true;
                }
            }

        return false;
        }

    //Uniform Cost Time
    void expandUCT()
        {
        bool goalStatus;

        if (!_black.empty())
            {
            //Check if goal has been reached
            pos = G->Graph.find((_black.front()).State);
            goalStatus = testGoal(pos->first);
            if (goalStatus == false)
                {
                //expand node
                //push it into visitedList
                _grey.push_back(_black.front());

                //Find friends of Node
                for (int i = 0; i < (pos->second).size(); i++)
                    {
                    //Check if the Node is already in Grey Q
                    if (!checkNodeinGrey((pos->second)[i].connectedTo))
                        {
                        if (!checkNodeinBlack((pos->second)[i].connectedTo))
                            {
                            _nodeNum += 1;

                            /*       NodeNumber,
                                          State,
                                          Depth,
                                          Cost,
                                          Parent.
                            */
                            _black.push_back(Node(_nodeNum,
                                             (pos->second)[i].connectedTo,
                                             ((_black.front()).Depth) + 1, (
                                                 (pos->second)[i].costTime +
                                                 (_black.front()).Cost),
                                             (_black.front()).nodeNumber));
                            }
                        else
                            {
                            //Get the node present in the Graph
                            Node n = getNodeinBlack((pos->second)[i].connectedTo);

                            //Calculate current nodes path cost.
                            int currentCost = (pos->second)[i].costTime +
                                (_black.front()).Cost;

                            //Compare the cost of current and the previous node
                            if (currentCost < n.Cost)
                                {
                                _nodeNum += 1;

                                //Remove the previous node and push current node.
                                if (removeNodeFromBlack(n, n.State))
                                    {
                                    _black.push_back(Node(_nodeNum, (pos->second)[i].connectedTo,
                                                         ((_black.front()).Depth) +
                                                                     1, (
                                                                         (pos->second)[i].costTime +
                                                                             (_black.front()
                                                                             ).Cost
                                                                             ),
                                                                                     (_black.front()).nodeNumber
                                                                                             ));
                                    }
                                }
                            }
                        }
                    }

                cout << endl;

                //Remove from black and color grey
                _black.pop_front();

                //Sort the list based on the cost
                _black.sort();
                _grey.sort();

                expandUCT();
                }
            else
                {
                //cout << "\nUniform-Cost Time path : ";
                printPath(pos->first, "sort");
                //cout << Path;
                bfsGraph.openFile("uniform-cost.time.result.txt");
                bfsGraph.writeToFile(Path);
                bfsGraph.closeFile();
                }
            }
        else
            {
            Path = "Recipient could not be found!";
            //cout << Path;
            bfsGraph.openFile("uniform-cost.time.result.txt");
            bfsGraph.writeToFile(Path);
            bfsGraph.closeFile();
            }
        }

    //Uniform Cost Risk
    void expandUCR()
        {
        bool goalStatus;

        if (!_black.empty())
            {
            //Check if goal has been reached
            pos = G->Graph.find((_black.front()).State);
            goalStatus = testGoal(pos->first);
            if (goalStatus == false)
                {
                //expand node
                //push it into visitedList
                _grey.push_back(_black.front());
                //Find friends of Node
                for (int i = 0; i < (pos->second).size(); i++)
                    {
                    //Check if the Node is already in Grey Q
                    if (!checkNodeinGrey((pos->second)[i].connectedTo))
                        {
                        if (!checkNodeinBlack((pos->second)[i].connectedTo))
                            {
                            _nodeNum += 1;

                            /*       NodeNumber,
                                          State,
                                          Depth,
                                          Cost,
                                          Parent.
                            */
                            _black.push_back(Node(_nodeNum,(pos->second)[i].connectedTo,((_black.front()).Depth) + 1,
                                                                     ((pos->second)[i].costRisk + (_black.front()).Cost),(_black.front()).nodeNumber));


                            }
                        else
                            {
                            //Get the node present in the Graph
                            Node n = getNodeinBlack((pos->second)[i].connectedTo);

                            //Calculate current nodes path cost.
                            int currentCost = (pos->second)[i].costRisk +(_black.front()).Cost;

                            //Compare the cost of current and the previous node
                            if (currentCost < n.Cost)
                                {
                                _nodeNum += 1;

                                //Remove the previous node and push current node.
                                if (removeNodeFromBlack(n, n.State))
                                    {
                                    _black.push_back(Node(_nodeNum, (pos->second)[i].connectedTo,((_black.front()).Depth) +1,
                                                                    ((pos->second)[i].costRisk +(_black.front()).Cost),(_black.front()).nodeNumber));

                                    }
                                }
                            }
                        }
                    }

                cout << endl;

                //Remove from black and color grey
                _black.pop_front();

                //Sort the list based on the cost
                _black.sort();
                _grey.sort();

                expandUCR();
                }
            else
                {
                //cout << "Uniform-Cost Search path: ";
                printPath(pos->first, "sort");
                //cout << Path;
                bfsGraph.openFile("uniform-cost.risk.result.txt");
                bfsGraph.writeToFile(Path);
                bfsGraph.closeFile();
                }
            }
        else
            {
            Path = "Recipient could not be found!";
            //cout << Path;
            bfsGraph.openFile("uniform-cost.risk.result.txt");
            bfsGraph.writeToFile(Path);
            bfsGraph.closeFile();
            }
        }
};

int main(int argc, char *argv[])
    {
    classGraph objGraph;
    Search objS(&objGraph);
    //cout << "Building Graph...\n\n";
    objGraph.buildGraph();
    objGraph.cleanGraph();
    //cout << "Graph Complete.\n\n";

    //cout<<"Search Started.\n";
    //objGraph.printGraph ();
    objS.setSender("Alice");
    objS.setRecipient("Noah");

    objS.startSearches();

    //cout << "Search Complete.\n\nEnter x to Exit" << endl;

    return EXIT_SUCCESS;
    }
