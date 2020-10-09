#ifndef PLATE_H
#define PLATE_H

namespace polaris
{
    //!  Base for Plate behavior.
    class Plate
    {
    public:
        virtual void Update(); //!< Called every tick while the plate is primary on the stack.

        virtual void OnEnabled(); //!< Called when the plate becomes primary on the stack.
        virtual void OnDisabled(); //!< Called when the plate is not primary on the stack anymore.
    };
}

#endif // PLATE_H