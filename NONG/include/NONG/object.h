#ifndef NONG_OBJECT_H
#define NONG_OBJECT_H

#include <vector>
#include <set>
#include <type_traits>

#include "NONG/component.h"

namespace NONG {
    class Object
    {
    private:
        static std::vector<Object*> objects;
        std::vector<Component*> components;

        std::string name;
        std::set<std::string> tags;
    public:

#pragma region Constructors
        Object();
        Object(std::string name);

        ~Object();
#pragma endregion

#pragma region Components
        template<class T, typename ...args>
        T* AddComponent(args...);
        bool RemoveComponent(Component* component);

        std::vector<Component*> GetAllComponents();
        Component* GetComponentOfType(const std::string& type);
        std::vector<Component*> GetAllComponentsOfType(const std::string& type);

        template <DerivedFromComponent T, typename... Args>
        T *AddComponent(Args... args);

        template <DerivedFromComponent T>
        T *GetComponent();

        template<DerivedFromComponent T>
        std::vector<T*> GetAllComponents();   
#pragma endregion

#pragma region Naming
        void SetName(const std::string& newName);
        std::string GetName() const;
        // TAGS

        bool AddTag(const std::string& tag);
        bool RemoveTag(const std::string& tag);
        bool HasTag(const std::string& tag) const;
        std::set<std::string> GetAllTags();
#pragma endregion


#pragma region Static functions
        // STATIC FUNCTIONS
        static std::vector<Object*> GetAllObjects();
        static Object* GetObjectByTag(const std::string& tag);
        static std::vector<Object*> GetAllObjectsByTag(const std::string& tag);
#pragma endregion
        
    };
}

#include "NONG/object.tpp"

#endif