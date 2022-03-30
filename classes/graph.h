#ifndef GRAPH_CONTAINRR
#define GRAPH_CONTAINRR

namespace containrr{
using namespace containrr;

    template<class T>
    struct connectionValuePair{
        T value;
        bool exists;
    };

    //Graph<NODE DATATYPE, CONNECTION DATATYPE> graph(VERTICES);
    template<class T=unsigned long long, class U=unsigned long long>
    class Graph{
        public:
            bool isDirected=true, **connection=nullptr;
            unsigned long long n;
            T *vStorinator=nullptr;
            U **connectionValue=nullptr;
            T *searchOutput=nullptr;
            connectionValuePair<U> *djikstraOutput=nullptr;

            void init(unsigned long long n, bool isDirected){
                this->n=n;
                this->isDirected=isDirected;
                vStorinator=new T[n];
                searchOutput=new T[n];
                djikstraOutput=new connectionValuePair<U>[n];
                connection=new bool *[n];
                for(unsigned long long i=0;i<n;i++)
                    connection[i]=new bool [n];
                connectionValue=new U *[n];
                for(unsigned long long i=0;i<n;i++)
                    connectionValue[i]=new U [n];
            }

            void connect(unsigned long long x, unsigned long long y, unsigned long long val){
                connectionValue[x][y]=val;
                connection[x][y]=true;
            }

            void disconnect(unsigned long long x, unsigned long long y){
                connection[x][y]=false;
            }
            
            void dfs(bool *visited, unsigned long long pos,  unsigned long long &index){
                searchOutput[index++]=vStorinator[pos];
                visited[pos]=true;
                for(int i=0;i<n;i++)
                    if(connection[pos][i]&&!visited[i])
                        dfs(visited, i, index);
            }

            T *dfs(unsigned long long pos){
                bool *visited=new bool [n];
                for(int i=0;i<n;i++)
                    searchOutput[i]=T();
                unsigned long long index=0;
                dfs(visited, pos, index);
                delete[] visited;
                return searchOutput;
            }

            T *bfs(unsigned long long pos){
                bool *visited=new bool [n];
                unsigned long long *positions=new unsigned long long [n];
                for(int i=0;i<n;i++)
                    searchOutput[i]=T();
                unsigned long long left=0,right=1;
                //bfs
                delete[] visited;
                delete[] positions;
                return searchOutput;
            }

            connectionValuePair<U> *djikstra(unsigned long long pos, bool (*comparison)(U, U)){
                bool *visited=new bool [n];
                for(int i=0;i<n;i++)
                    djikstraOutput[i].value=T();
                visited[pos]=true;
                for(unsigned long long i=0;i<n;i++){
                    djikstraOutput[i].exists=connection[pos][i];
                    djikstraOutput[i].value=connectionValue[pos][i];
                }
                for(unsigned long long k=0;k<n;k++){
                    pos=-1ull;
                    for(unsigned long long i=0;i<n;i++)
                        if(djikstraOutput[i].exists==true&&!visited[i]&&(pos==-1ull||comparison(djikstraOutput[pos].value, djikstraOutput[i].value)))
                            pos=i;
                    if(pos!=-1ull){
                        visited[pos]=true;
                        for(unsigned long long i=0;i<n;i++)
                            if(connection[pos][i]&&!visited[i]){
                                if(!djikstraOutput[i].exists){
                                    djikstraOutput[i].exists=true;
                                    djikstraOutput[i].value=connectionValue[pos][i]+djikstraOutput[pos].value;
                                }else if(comparison(djikstraOutput[i].value, connectionValue[pos][i]+djikstraOutput[pos].value))
                                    djikstraOutput[i].value=connectionValue[pos][i]+djikstraOutput[pos].value;
                            }
                    }
                }
                delete[] visited;
                return djikstraOutput;
            }

        //public:
            Graph(unsigned long long n, bool isDirected=true){
                init(n, isDirected);
            }

            T *test(int val){
                return dfs(val);
            }

            void setNodeVal(int val, int pos){
                vStorinator[pos]=val;
            }

            void connectForTesting(int x, int y){
                connect(x,y,1);
            }

            void del(){
                for(int i=0;i<n;i++)
                    delete[] connectionValue[i];
                delete[] connectionValue;
                for(int i=0;i<n;i++)
                    delete[] connection[i];
                delete[] connection;
                delete[] searchOutput;
                delete[] djikstraOutput;
                delete[] vStorinator;
            }


    };
}

#endif