#ifndef COMMANDS_GRAPH_CONTAINRR
#define COMMANDS_GRAPH_CONTAINRR
#include "command.h"

namespace containrr{
    using namespace containrr;

    //returns a pointer to an array with the solution(a list of vertice values)
    Command<void> dfs(){
        Command<void> command;
        command.id=5;
        command.n=0;
        return command;
    }

    //returns a pointer to an array with the solution(a list of vertice values)
    Command<void> bfs(){
        Command<void> command;
        command.id=6;
        command.n=0;
        return command;
    } 

    //returns a pointer to an array with the solution(a list of connection
    //lengths paired with bools that signify the existance of the paired connection)
    //auto *ARRAY_NAME=v[POS](dijkstra())
    template <typename U>
    Command<bool (*)(U, U)> dijkstra(bool (*val)(U, U)){
        Command<bool (*)(U, U)> command;
        command.id=7;
        command.vals=new (bool (*)(U, U));
        *command.vals=val;
        command.n=0;
        return command;
    }

    //returns a pointer to an array with the solution(a list of connection
    //lengths paired with bools that signify the existance of the paired connection)
    //auto *ARRAY_NAME=v[POS](dijkstra());
    Command<Command<void>> dijkstra(){
        Command<Command<void>> command;
        command.id=7;
        command.n=0;
        return command;
    }
}
#endif