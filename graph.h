#ifndef GRAPH_CONTAINRR
#define GRAPH_CONTAINRR

namespace containrr{
using namespace containrr;

    template <class T=int, class U=int>
    class Graph{
        private:
            T **storinator=nullptr, *pathData=nullptr;
            U *customNodeData=nullptr, *searchResults=nullptr;
            unsigned long long *graphInternalConnections=nullptr, *graphExternalConnections=nullptr;
            unsigned long long n=0, index=0;
            bool isOriented=false, searchIsCached=false;

            void runDfs(int pos, bool *visited){
                visited[pos]=true;
                searchResults[index++]=customNodeData[pos];
                for(int i=0;i<n;i++)
                    if(storinator[pos][i]&&!visited[i])
                        runDfs(i,visited);
            }

            void runBfs(int pos, bool *visited){
                searchResults[index++]=customNodeData[pos];
                for(unsigned long long i=0;i<index;i++)
                    for(unsigned long long j=0;j<n;j++)
                        if(storinator[i][j]&&!visited[i]){
                            visited[i]=true;
                            searchResults[index++]=customNodeData[pos];
                        }
            }

            void dijkstra(int pos){
                //de implementat dijkstra cu heap
            }

        public:

            Graph(unsigned long long n, bool isOriented=false, T defaultConnectionValue=0){
                this->n=n;
                this->isOriented=isOriented;
                storinator=new T *[n];
                for(int i=0;i<n;i++)
                    storinator[i]=new T[n];
                pathData=new T[n];
                customNodeData=new U[n];
                searchResults=new U[n];
                if(defaultConnectionValue!=0)
                    for(unsigned long long i=0;i<n;i++)
                        for(unsigned long long j=0;j<n;j++)
                            if(i!=j)
                                storinator[i][j]=defaultConnectionValue;
                graphExternalConnections=new unsigned long long [n];
                if(isOriented){
                    graphInternalConnections=new unsigned long long [n];
                }
            }

            void del(){
                for(int i=0;i<n;i++)
                    delete[] storinator[i];
                delete[] storinator;
                delete[] graphExternalConnections;
                delete[] graphInternalConnections;
                delete[] pathData;
                delete[] searchResults;
                delete[] customNodeData;
            }

            Graph *operator[](unsigned long long index){
                this->index=index;
                return this;
            }

            void operator=(U val){
                if(index==0)
                    return;
                customNodeData[index]=val;
                index=0;
            }

            void connect(unsigned long long pos, T connectionData=1){
                if(index==0)
                    return;
                searchIsCached=false;
                if(pos!=index){
                    storinator[index][pos]=connectionData;
                    if(isOriented==false)
                        storinator[pos][index]=connectionData;
                }
                index=0;
            }

            void disconnect(unsigned long long pos){
                if(index==0)
                    return;
                storinator[index][pos]=0;
                index=0;
            }

            U *dfs(){
                for(int i=0;i<n;i++)
                    searchResults[i]=0;
                if(index==0)
                    return searchResults;
                bool *visited=new bool[n];
                unsigned long long temp=index;
                index=0;
                runDfs(temp, visited);
                index=0;
                delete[] visited;
                return searchResults;
            }

            U *bfs(){
                for(int i=0;i<n;i++)
                    searchResults[i]=0;
                if(index==0)
                    return searchResults;
                bool *visited=new bool[n];
                unsigned long long temp=index;
                index=0;
                runBfs(temp, visited);
                index=0;
                delete[] visited;
                return searchResults;
            }

            T *getPathLenghts(){
                for(int i=0;i<n;i++)
                    pathData[i]=-1;
                if(index>n)
                    return pathData;
                bool *visited=new bool[n];
                unsigned long long temp=index;
                index=0;
                dijkstra(temp);
                index=0;
                delete[] visited;
                return pathData;
            }

    };
}

#endif