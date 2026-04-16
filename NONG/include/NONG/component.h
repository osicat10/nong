#ifndef NONG_COMPONENT_H
#define NONG_COMPONENT_H

#define DECLARE_COMPONENT(Type) \
    public: \
        static std::string GetStaticType() { return #Type; } \
        virtual std::string GetType() const override { return GetStaticType(); }

#include <string>
#include <vector>

namespace NONG {
    class Component
    {
    public:
        virtual std::string GetType() const = 0;

        virtual ~Component() = default;
    };

    class MonoBehaviour : public Component
    {
        DECLARE_COMPONENT(MonoBehaviour)
    private:
        static std::vector<MonoBehaviour*> newMonoBehaviours;
        static void StartNewMonoBehaviours();
    public:
        MonoBehaviour();
        virtual void Start();
        virtual void Update();
    };
}

#endif