#include "NONG/object.h"
#include <algorithm>

namespace NONG {
    std::vector<Object*> Object::objects;

    Object::Object() : Object("Object") { }
    Object::Object(std::string name) : name(name) 
    {
        objects.push_back(this);
    }
    Object::~Object()
    {
        for(Component* c : components)
        {
            delete c;
        }

        objects.erase(std::find(objects.begin(), objects.end(), this));
    }

    bool Object::RemoveComponent(Component* component)
    {
        if(auto it = std::find_if(components.begin(), components.end(), 
            [&component](const Component* c) { return c == component; }); 
            it != components.end())
        {
            Component* ptr = *it;
            delete ptr;
            monoBehaviours.erase(dynamic_cast<MonoBehaviour*>(ptr));
            components.erase(it);         

            return true;
        }
        return false;
    }

    std::vector<Component*> Object::GetAllComponents()
    {
        std::vector<Component*> _cs;
        for(auto& component : components)
        {
            _cs.push_back(component);
        }
        return _cs;
    }

    Component* Object::GetComponentOfType(const std::string& type)
    {
        if(auto it = std::find_if(components.begin(), components.end(), 
            [&type](const Component* c) { return c->GetType() == type; }); 
            it != components.end())
        {
            return *it;
        }
        return nullptr;
    }

    std::vector<Component*> Object::GetAllComponentsOfType(const std::string& type)
    {
        std::vector<Component*> _cs; 
        for(auto& component : components)
        {
            if(component->GetType() == type)
            {
                _cs.push_back(component);
            }
        }

        return _cs;
    }

    void Object::SetName(const std::string& newName)
    {
        name = newName;
    }

    std::string Object::GetName() const
    {
        return name;
    }

    bool Object::AddTag(const std::string& tag)
    {
        return tags.insert(tag).second;
    }

    bool Object::RemoveTag(const std::string& tag)
    {
        return tags.erase(tag);
    }

    bool Object::HasTag(const std::string& tag) const
    {
        return tags.count(tag);
    }

    std::set<std::string> Object::GetAllTags()
    {
        return tags;
    }

    std::vector<Object*> Object::GetAllObjects()
    {
        return objects;
    }

    Object* Object::GetObjectByTag(const std::string& tag)
    {
        if(auto it = std::find_if(objects.begin(), objects.end(), 
            [&tag](const Object* o) { return o->HasTag(tag); }); 
            it != objects.end())
        {
            return *it;
        }
        return nullptr;
    }
    std::vector<Object*> Object::GetAllObjectsByTag(const std::string& tag)
    {
        std::vector<Object*> _os;
        for(auto& o : objects)
        {
            if(o->HasTag(tag))
            {
                _os.push_back(o);
            } 
        }
        return _os;
    }

    void Object::RunMonoBehaviours()
    {
        for(MonoBehaviour* mb : monoBehaviours)
        {
            mb->Update();
        }
    }
}