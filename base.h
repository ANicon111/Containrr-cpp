/* Copyright (C) 2022 Albu Nicon Georgian.

This file is part of the containrr library.

The containrr library is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

The containrr library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

<http://www.gnu.org/licenses/>.  */

#ifndef BASE_CONTAINRR
#define BASE_CONTAINRR

namespace containrr{
    using namespace containrr;

    struct Interval{
        unsigned long long start=0, end=0;
        bool onlyStart=false;

        //Empty constructor
        //It is used for easy creation of Interval arrays.
        Interval(){}

        //An interval from the start START to the end of the axis
        Interval(unsigned long long start){
            this->start=start;
            onlyStart=true;
        }

        //An interval from the start START to the end END.
        Interval(unsigned long long start, unsigned long long end){
            this->start=start;
            this->end=end;
        }

        //Returns false if END is lower than START or if either is larger than MAXINDEX or lower than 0 (uses an overflow)
        bool isValid(unsigned long long maxIndex=-1){
            if(start>=maxIndex)
                return false;
            if(end>maxIndex)
                return false;
            if(start>end)
                return false;
            return true;
        }
    };
}

#endif