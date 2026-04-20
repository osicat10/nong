#include "object.h"
namespace NONG {

    template<DerivedFromComponent T, typename ...Args>
    T* Object::AddComponent(Args... args)
    {
        T* c = new T(args...);
        Component* component = dynamic_cast<Component*>(c);
        components.push_back(component);
        component->holder = this;

        return c;
    }

    template<DerivedFromComponent T>
    T* Object::GetComponent()
    {
        for(auto& component : components)
        {
            if(auto c = dynamic_cast<T*>(component); c)
            {
                return c;
            }
        }
        return nullptr;
    }

    template<DerivedFromComponent T>
    std::vector<T*> Object::GetAllComponents()
    {
        std::vector<T*> _cs; 
        for(auto& component : components)
        {
            if(auto& c =  dynamic_cast<T*>(component); c)
            {
                _cs.push_back(c);
            }
        }

        return _cs;
    }   
}