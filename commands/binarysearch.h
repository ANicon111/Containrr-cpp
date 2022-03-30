#ifndef COMMANDS_BINARYSEARCH_CONTAINRR
#define COMMANDS_BINARYSEARCH_CONTAINRR
#include "command.h"
#include "interval.h"

namespace containrr{
    using namespace containrr;
    
    //v[lowerBound(VALUE, INTERVAL)
    //
    //Sets an interval of the lower bound of a value on this axis
    //The placeholder interval until the command is processed is Interval(0,1)
    //The command can be constrained within the interval INTERVAL
    template <typename T>
    Command<T> lowerBound(T val, Interval interval=Interval(0)){
        Command<T> command;
        command.id=1;
        command.vals=new T[1];
        command.vals[0]=val;
        command.n=1;
        command.interval=interval;
        return command;
    }

    //v[upperBound(VALUE, INTERVAL)
    //
    //Sets an interval of the upper bound of a value on this axis
    //The placeholder interval until the command is processed is Interval(0,1)
    //The command can be constrained within the interval INTERVAL
    template <typename T>
    Command<T> upperBound(T val, Interval interval=Interval(0)){
        Command<T> command;
        command.id=2;
        command.vals=new T[1];
        command.vals[0]=val;
        command.n=1;
        command.interval=interval;
        return command;
    }
}
#endif