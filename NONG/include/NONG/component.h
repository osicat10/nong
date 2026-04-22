#ifndef NONG_COMPONENT_H
#define NONG_COMPONENT_H

#define DECLARE_COMPONENT(Type) \
    public: \
        static std::string GetStaticType() { return #Type; } \
        virtual std::string GetType() const override { return GetStaticType(); }

#include <string>
#include <vector>

namespace NONG {
    class Object;
    
    class Component
    {
    private:
        Object* holder;
    public:
        virtual std::string GetType() const = 0;

        virtual ~Component() = default;
        Object* GetObject();

    friend class Object;
    };

    class MonoBehaviourController
    {
    public:
        static void RunMonoBehaviours();
        static void StartNewMonoBehaviours();
    };

    class MonoBehaviour : public Component
    {
        DECLARE_COMPONENT(MonoBehaviour)
    private:
        static std::vector<MonoBehaviour*> monoBehaviours;
        static std::vector<MonoBehaviour*> newMonoBehaviours;
        static void RunMonoBehaviours();
        static void StartNewMonoBehaviours();

        bool enabled = true;
        
    public:
        MonoBehaviour();
        MonoBehaviour(bool enabled);
        ~MonoBehaviour();
        virtual void Start();
        virtual void Update();
        virtual void LateUpdate();

        void SetEnabled(bool enabled);
        bool IsEnabled();

    friend class MonoBehaviourController;
    };

    template<class T>
    concept DerivedFromComponent = std::is_base_of_v<Component, T>;
}

#endif