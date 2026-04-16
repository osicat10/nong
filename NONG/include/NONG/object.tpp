namespace NONG {
    template<class T, typename ...Args>
    T* Object::AddComponent(Args... args)
    {
        T* c = new T(args...);
        components.push_back(dynamic_cast<Component*>(c));

        if constexpr (std::is_base_of<MonoBehaviour, T>::value) 
        {
            MonoBehaviour* mb = dynamic_cast<MonoBehaviour*>(c);
            monoBehaviours.insert(mb);
        }

        return c;
    }

    template<class T>
    T* Object::GetComponent()
    {
        for(auto& component : components)
        {
            if(auto& c =  dynamic_cast<T*>(component); c)
            {
                return c;
            }
        }
        return nullptr;
    }

    template<class T>
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