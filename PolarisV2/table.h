#ifndef TABLE_H
#define TABLE_H

#include "plate.h"

#include <list>

namespace polaris
{
    //!  A stacked State Machine class.
    /*!
    * Stacks Plates on top of each other, the plate on top of the stack is considered Primary and will execute.
    */
    class Table
    {
    private:
        std::list<Plate*> m_lPlates; // NOTE: The plate at 0 is considered primary.

    public:
        void Update(); //!< Update has to be called to keep the Plates functioning properly, preferrably at a (1000 / 60) interval.

        void PushPlate(Plate* plate); //!< PushPlate adds a new plate on top of the stack.
        void PopPlate(); //!< PopPlate pops the primary plate, making the plate below primary.
        void Pop(); //!< Pop pops all the plates and throws the Table into the garbage collector.
    };
}

#endif // TABLE_H