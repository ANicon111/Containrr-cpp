#ifndef GRAPH_CONTAINRR
#define GRAPH_CONTAINRR
#include "../commands/command.h"

namespace containrr{
using namespace containrr;

    template<class T>
    struct ConnectionValuePair{
        T value;
        bool exists;
    };

    //v[X](connect(Y, VAL))
    //connects nodes X and Y
    template <typename T>
    Command<T> connect(unsigned long long pos, T val){
        Command<T> command;
        command.id=3;
        command.vals=new T[1];
        command.vals[0]=val;
        command.n=pos;
        return command;
    }

    //v[X](connect(Y, VAL))
    //connects nodes X and Y
    Command<void> connect(unsigned long long pos){
        Command<void> command;
        command.id=3;
        command.n=pos;
        return command;
    }

    //v[X](disconnect(Y))
    //disconnects nodes X and Y
    Command<void> disconnect(unsigned long long pos){
        Command<void> command;
        command.id=4;
        command.n=pos;
        return command;
    }

    template<class T=unsigned long long, class U=unsigned long long>
    class Graph{
        private:
        bool isDirected=true, **connection=nullptr;
        unsigned long long n, selectedIndex=-1ull;
        T *vStorinator=nullptr;
        U **connectionValue=nullptr;
        T *searchOutput=nullptr;
        ConnectionValuePair<U> *djikstraOutput=nullptr;

        void init(unsigned long long n, bool isDirected){
            this->n=n;
            this->isDirected=isDirected;
            vStorinator=new T[n];
            searchOutput=new T[n];
            djikstraOutput=new ConnectionValuePair<U>[n];
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
            if(!isDirected){
                connectionValue[y][x]=val;
                connection[y][x]=true;
            }
        }

        void disconnect(unsigned long long x, unsigned long long y){
            connection[x][y]=false;
            if(!isDirected)
                connection[y][x]=true;
        }
        
        void dfs(bool *visited, unsigned long long pos,  unsigned long long &index){
            searchOutput[index++]=vStorinator[pos];
            visited[pos]=true;
            for(int i=0;i<n;i++)
                if(connection[pos][i]&&!visited[i])
                    dfs(visited, i, index);
        }

        void dfs(unsigned long long pos){
            bool *visited=new bool [n];
            for(int i=0;i<n;i++)
                searchOutput[i]=T();
            unsigned long long index=0;
            dfs(visited, pos, index);
            delete[] visited;
        }

        void bfs(unsigned long long pos){
            bool *visited=new bool [n];
            unsigned long long *positions=new unsigned long long [n];
            for(int i=0;i<n;i++)
                searchOutput[i]=T();
            unsigned long long left=0,right=1;
            visited[pos]=true;
            positions[0]=pos;
            searchOutput[0]=vStorinator[pos];
            while(left<right){
                for(int i=0;i<n;i++)
                    if(connection[positions[left]][i]&&!visited[i]){
                        positions[right]=i;
                        searchOutput[right]=vStorinator[i];
                        visited[i]=true;
                        right++;
                    }
                left++;
            }
            delete[] visited;
            delete[] positions;
        }

        void djikstra(unsigned long long pos, bool (*comparison)(U, U)){
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
        }

        public:
        //Graph<VERTICE_TYPE, CONNECTION_TYPE> graph(SIZE, IS_DIRECTIONAL);
        //
        //The CONNECTION_TYPE has to have the + operator overloaded
        //The memory complexity is about 9*SIZE^2 bits
        Graph(unsigned long long n, bool isDirected=true){
            init(n, isDirected);
        }

        Graph operator[](unsigned long long pos){
            selectedIndex=pos;
            return *this;
        }

        T *operator()(Command<U> command){
            if(selectedIndex<n&&command.n!=selectedIndex&&command.n<n)
                switch (command.id){
                case 3:
                    connect(selectedIndex, command.n, command.vals[0]);
                    delete[] command.vals;
                    break;
                default:
                    break;
                }
            selectedIndex=-1ull;
            return nullptr;
        }

        T *operator()(Command<void> command){
            if(selectedIndex<n&&command.n!=selectedIndex&&command.n<n)
                switch (command.id){
                case 3:
                    connect(selectedIndex, command.n, U());
                    break;
                case 4:
                    disconnect(selectedIndex, command.n);
                    break;
                case 5:
                    dfs(selectedIndex);
                    return searchOutput;
                    break;
                case 6:
                    bfs(selectedIndex);
                    return searchOutput;
                    break;
                default:
                    break;
                }
            selectedIndex=-1ull;
            return nullptr;
        }

        ConnectionValuePair<U> *operator()(Command<bool (*)(U, U)> command){
            if(selectedIndex<n)
                switch (command.id){
                case 7:
                    djikstra(selectedIndex, *command.vals);
                    delete command.vals;
                    return djikstraOutput;
                    break;
                default:
                    break;
                }
            selectedIndex=-1ull;
            return nullptr;
        }

        ConnectionValuePair<U> *operator()(Command<Command<void>> command){
            if(selectedIndex<n)
                switch (command.id){
                case 7:
                    djikstra(selectedIndex, defaultComparison);
                    return djikstraOutput;
                    break;
                default:
                    break;
                }
            selectedIndex=-1ull;
            return nullptr;
        }

        void operator=(T val){
            if(selectedIndex<n)
                vStorinator[selectedIndex]=val;
        }
        
        //Delete the Graph
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