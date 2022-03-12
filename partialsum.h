#ifndef PARTIALSUM_CONTAINRR
#define PARTIALSUM_CONTAINRR
#include "base.h"
#include "commands/base.h"

namespace containrr{
using namespace containrr;

    template <class T=int>
    class PartialSum{
        private:
        unsigned long long *lengths=nullptr, *i=nullptr;
        T *storinator=nullptr;
        Command<T> *commands;
        Interval *intervals;
        short n=0, index=0, intervalToReturn=0;

        void init(){
            i=new unsigned long long[n];
            intervals = new Interval[n];
            commands = new Command<T>[n];
            unsigned long long size=1;
            for(short i=0;i<n;i++)
                size*=lengths[i];
            storinator = new T[size];
        }
        template<typename IntType, typename... Types>
        void init(IntType length, Types... lengths){
            this->lengths[n++]=length;
            init(lengths...);
        }
        void getN(){return;}
        template<typename IntType, typename... Types>
        void getN(IntType length, Types... lengths){
            n++;
            getN(lengths...);
        }

        T getVal(){
            unsigned long long pos=0, factor=1;
            for(short i=0;i<n;i++){
                pos+=this->i[i]*factor;
                factor*=lengths[i];
            }
            if(pos<factor)
                return storinator[pos];
            else
                return 0;
        }
        void setVal(T val){
            unsigned long long pos=0, factor=1;
            for(int i=0;i<n;i++){
                pos+=this->i[i]*factor;
                factor*=lengths[i];
            }
            if(pos<factor)
                storinator[pos]=val;
        }
        void addVal(T val){
            unsigned long long pos=0, factor=1;
            for(int i=0;i<n;i++){
                pos+=this->i[i]*factor;
                factor*=lengths[i];
            }
            if(pos<factor)
                storinator[pos]+=val;
        }
        bool stateIsValid(){
            if(index!=n)
                return false;
            for(short i=0;i<n;i++){
                if(!intervals[i].isValid(lengths[i]))
                    return false;
                if(commands[i].id){
                    if(commands[i].id!=1&&commands[i].id!=2)
                        return false;
                    if(!commands[i].interval.isValid(lengths[i]))
                        return false;
                }
            }
            return true;
        }
        void reset(){
            index=0;
            intervalToReturn=0;
            for(int i=0;i<n;i++){
                this->i[i]=0;
                intervals[i]=Interval(0);
                commands[i].id=0;
                if(commands[i].vals)
                    delete commands[i].vals;
                commands[i].vals=nullptr;
            }
        }

        unsigned long long getBiggestPow(unsigned long long val){
            unsigned long long pow=1;
            while((pow<<1)<=val)
                pow<<=1;
            return pow;
        }

        void add(T x, short dim){
            unsigned long long pow=1, oldI=i[dim];
            while(i[dim]<lengths[dim]){
                if((i[dim]+1)%pow){
                    if(dim>0){
                        add(x, dim-1);
                    }else{
                        addVal(x);
                    }
                    i[dim]+=pow>>1;
                }
                pow=pow<<1;
            }
            i[dim]=oldI;
        }

        void addInterval(T x, short dim, bool equals=false){
            unsigned long long start=intervals[dim].start, end=intervals[dim].end;
            for(unsigned long long j=start;j<end;j++){
                Interval temp=intervals[dim];
                i[dim]=j;
                intervals[dim]=Interval(j, j+1);
                if(dim==0){
                    T val=x;
                    if(equals)
                        val-=getInterval();
                    add(val,n-1);
                }else
                    addInterval(x,dim-1,equals);
                intervals[dim]=temp;
            }
        }

        T get(short dim){
            unsigned long long end=intervals[dim].end, oldI=i[dim];
            T sum=0;
            i[dim]=-1;
            for(unsigned long long pow=getBiggestPow(lengths[dim]);pow>0;pow=pow>>1){
                if(i[dim]+pow<end){
                    i[dim]+=pow;
                    dim==0 ? sum+=getVal() : sum+=get(dim-1);
                }
                
            }
            i[dim]=oldI;
            return sum;
        }

        T getInterval(short i=0){
            T val=0;
            val+=get(n-1);
            for(;i<n;i++){
                unsigned long long temp=intervals[i].end;
                intervals[i].end=intervals[i].start;
                val-=getInterval(i+1);
                intervals[i].end=temp;
            }
            return val;
        }

        void getLowerBound(T val, short dim){
            bool optimized=true;
            if(commands[dim].interval.start>0)
                optimized=false;
            for(short i=0;i<n;i++)
                if(intervals[i].end>1){
                        optimized=false;
                        break;
                }
            intervals[dim].start=commands[dim].interval.start;
            if(optimized){
                i[dim]=-1;
                T sum=0;
                for(unsigned long long pow=getBiggestPow(commands[dim].interval.end);pow>0&&i[dim]+pow>=commands[dim].interval.start;pow=pow>>1){
                    i[dim]+=pow;
                    sum+=getVal();
                    if(i[dim]>=commands[dim].interval.end||sum>=val){
                        sum-=getVal();
                        i[dim]-=pow;
                    }
                }
                intervals[dim].end=i[dim]+1;
            }else{
                intervals[dim].end=0;
                for(unsigned long long pow=getBiggestPow(commands[dim].interval.end);pow>0&&intervals[dim].end+pow>commands[dim].interval.start;pow=pow>>1){
                    intervals[dim].end+=pow;
                    if(intervals[dim].end>=commands[dim].interval.end||getInterval()>=val)
                        intervals[dim].end-=pow;
                }
            }
            if(intervals[dim].end==0)
                intervals[dim].end=intervals[dim].start;
            intervals[dim].end++;
        }

        void getUpperBound(T val, short dim){
            bool optimized=true;
            if(commands[dim].interval.start>0)
                optimized=false;
            for(short i=0;i<n;i++)
                if(intervals[i].end>1){
                        optimized=false;
                        break;
                }
            if(optimized){
                intervals[dim].start=commands[dim].interval.start;
                i[dim]=-1;
                T sum=0;
                for(unsigned long long pow=getBiggestPow(commands[dim].interval.end);pow>0&&i[dim]+pow>=commands[dim].interval.start;pow=pow>>1){
                    i[dim]+=pow;
                    sum+=getVal();
                    if(i[dim]>=commands[dim].interval.end||sum>val){
                        sum-=getVal();
                        i[dim]-=pow;
                    }
                }
                intervals[dim].end=i[dim]+1;
            }else{
                intervals[dim].start=commands[dim].interval.start;
                intervals[dim].end=0;
                for(unsigned long long pow=getBiggestPow(commands[dim].interval.end);pow>0&&intervals[dim].end+pow>commands[dim].interval.start;pow=pow>>1){
                    intervals[dim].end+=pow;
                    if(intervals[dim].end>=commands[dim].interval.end||getInterval()>val)
                        intervals[dim].end-=pow;
                }
            }
            if(intervals[dim].end==0)
                intervals[dim].end=intervals[dim].start;
            intervals[dim].end++;
        }

        void evalCommands(){
            for(short i=0;i<n;i++)
                if(commands[i].id)
                    switch (commands[i].id)
                    {
                    case 1:
                        intervalToReturn=i;
                        if(commands[i].interval.onlyStart)
                            commands[i].interval.end=lengths[i];
                        getLowerBound(commands[i].vals[0], i);
                        break;
                    case 2:
                        intervalToReturn=i;
                        if(commands[i].interval.onlyStart)
                            commands[i].interval.end=lengths[i];
                        getUpperBound(commands[i].vals[0], i);
                        break;
                    default:
                        break;
                    }
        }

        public:
        //PartialSum<TYPE> partialSum(DIMENSIONS);
        //
        //Creates a partial sum with type TYPE and DIMENSIONS(a list of unsigned variables)
        //Make sure TYPE can store the sum of all elements.
        //TYPE also has to have the = and += operator support, and has to be able to be set to 0.
        template<typename... Types>
        PartialSum(Types... lengths){
            getN(lengths...);
            this->lengths=new unsigned long long[n];
            n=0;
            init(lengths...);
        }

        //Gets/Sets the memory value at the selected position
        //To get the actual value just use the PartialSum just like any array/matrix
        T debugval(T newval=0){
            if(stateIsValid()){
                if(newval){
                    setVal(newval);
                    return 0;
                }
                return getVal();
            }
            return 0;
        }

        //Returns the current operation result
        operator T(){
            T val=0;
            if(stateIsValid())
                val=getInterval();
            reset();
            return val;
        }

        //Returns the last command's generated Interval
        operator Interval(){
            evalCommands();
            Interval interval=intervals[intervalToReturn];
            reset();
            return interval;
        }

        //Set a position for this axis
        //
        //Example: v[5]=10;
        PartialSum &operator[](long long pos) {
            intervals[index++]=Interval(pos,pos+1);
            return *this;
        }

        //Set an interval on this axis
        //
        //Example: TYPE val = v[Interval(5,10)];
        PartialSum &operator[](Interval pos) {
            intervals[index]=Interval(pos.start,pos.onlyStart?lengths[index]:pos.end);
            index++;
            return *this;
        }

        //Set a command on this axis
        //
        //Example: Interval valPos = v[lowerBound(val)];
        PartialSum &operator[](Command<T> command) {
            intervals[index]=Interval(0,1);
            commands[index++]=command;
            return *this;
        }

        //Add to the value of the current position
        //
        //Complexity: O( logn * logm * ... )
        void operator+=(T x){
            evalCommands();
            if(stateIsValid())
                addInterval(x,n-1);
            reset();
        }

        //Substract from the value of the current position
        //
        //Complexity: O( logn * logm * ... )
        void operator-=(T x){
            evalCommands();
            if(stateIsValid())
                addInterval(-x,n-1);
            reset();
        }

        //Set the value of the current position
        //
        //Complexity: O( 2logn * 2logm * ... )
        //Always use += or -= if possible
        void operator=(T x){
            evalCommands();
            if(stateIsValid()){
                addInterval(x,n-1,true);
            reset();
            }
        }

        //Get a pointer to the data array
        T *memDump(){
            return storinator;
        }

        //Delete the PartialSum
        void del(){
            reset();
            delete[] storinator;
            delete[] intervals;
            delete[] commands;
            delete[] lengths;
            delete[] i;
        }
    };
}
#endif