#ifndef TABLE_H
#define TABLE_H

#include "plate.h"

#include <list>

namespace polaris
{
    //!  A stacked State Machine class.
    /*!
    * Stacks Plates on top of each other, the Plate on top of the stack is considered Primary and will execute.
    */
    class Table
    {
    public:
        std::list<Plate*> m_lpPlates; //!< List of Plates. Plate at position 0 in list is considered Primary.

        void Update(); //!< Update has to be called to keep the Plates functioning properly, preferrably at a (1000 / 60) interval.

        void PushPlate(Plate* plate); //!< PushPlate adds a new Plate on top of the stack.
        void PopPlate(); //!< PopPlate pops the primary Plate, making the plate below primary.
        void Pop(); //!< Pop pops all the Plates and throws the Table into the garbage collector.
    };
}

#endif // !TABLE_H