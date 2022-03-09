#ifndef COMMANDS_BASE_CONTAINRR
#define COMMANDS_BASE_CONTAINRR
#include "../base.h"

namespace containrr{
    using namespace containrr;

    template<class T>
    struct Command{
        short id=0;
        Interval interval=Interval(0);
        unsigned long long n=0;
        T *vals=nullptr;

        //Empty constructor
        //It is used for easy creation of Command arrays.
        Command<T>(){}

        void del(){
            delete vals;
        }

        //Value convertor
        //Automatically converts values to another type
        //Types have to be compatible
        template<typename U>
        operator Command<U>(){
            Command<U> newCommand;
            newCommand.id=id;
            newCommand.interval=interval;
            newCommand.n=n;
            newCommand.vals=new U[n];
            for(short i=0;i<n;i++)
                newCommand.vals[i]=vals[i];
            delete vals;
            return newCommand;
        }
    };
}
#endif