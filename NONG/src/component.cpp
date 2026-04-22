#include "NONG/component.h"

#include <algorithm>

namespace NONG {
    Object* Component::GetObject()
    {
        return holder;
    }

    void MonoBehaviour::Start() { }
    void MonoBehaviour::Update() { }
    void MonoBehaviour::LateUpdate() { }

    std::vector<MonoBehaviour*> MonoBehaviour::newMonoBehaviours;
    std::vector<MonoBehaviour*> MonoBehaviour::monoBehaviours;

    MonoBehaviour::MonoBehaviour() : MonoBehaviour(true) { }
    MonoBehaviour::MonoBehaviour(bool enabled) : enabled(enabled)
    {
        newMonoBehaviours.push_back(this);
        monoBehaviours.push_back(this);
    }
    MonoBehaviour::~MonoBehaviour()
    {
        monoBehaviours.erase(std::find(monoBehaviours.begin(), monoBehaviours.end(), this));
    }

    void MonoBehaviour::StartNewMonoBehaviours()
    {
        if(newMonoBehaviours.empty()) return;

        static std::vector<MonoBehaviour*> monoBehavioursToStart;

        int writeIndex = 0;

        for(int readIndex = 0; readIndex < newMonoBehaviours.size(); readIndex++)
        {
            MonoBehaviour* mb = newMonoBehaviours[readIndex];

            if(mb->enabled)
            {
                monoBehavioursToStart.push_back(mb);
            }
            else
            {
                newMonoBehaviours[writeIndex] = mb;
                writeIndex++;
            }
        }

        newMonoBehaviours.erase(newMonoBehaviours.begin() + writeIndex, newMonoBehaviours.end());

        for(MonoBehaviour* mb : monoBehavioursToStart)
        {
            mb->Start();
        }
        monoBehavioursToStart.clear();
    }

    void MonoBehaviour::RunMonoBehaviours()
    {
        for(MonoBehaviour* mb : monoBehaviours)
        {
            if(mb->enabled) mb->Update();
        }

        for(MonoBehaviour* mb : monoBehaviours)
        {
            if(mb->enabled) mb->LateUpdate();
        }
    }

    void MonoBehaviour::SetEnabled(bool enabled)
    {
        this->enabled = enabled;
    }

    bool MonoBehaviour::IsEnabled()
    {
        return enabled;
    }

    void MonoBehaviourController::RunMonoBehaviours()
    {
        MonoBehaviour::RunMonoBehaviours();
    }

    void MonoBehaviourController::StartNewMonoBehaviours()
    {
        MonoBehaviour::StartNewMonoBehaviours();
    }

}