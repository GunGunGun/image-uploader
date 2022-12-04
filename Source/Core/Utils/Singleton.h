#ifndef IU_CORE_UTILS_SINGLETON_H
#define IU_CORE_UTILS_SINGLETON_H

#include "CoreTypes.h"

template<class T> class Singleton {
    public:
        Singleton<T>() {

        }

        virtual ~Singleton<T>() {

        }

        static T* instance() {
            static T ins;
            return &ins;
        }

    private:
        DISALLOW_COPY_AND_ASSIGN(Singleton<T>);
};


#endif