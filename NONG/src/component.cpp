#include "NONG/component.h"

namespace NONG {
    void MonoBehaviour::Start() { }
    void MonoBehaviour::Update() { }

    std::vector<MonoBehaviour*> newMonoBehaviours;

    MonoBehaviour::MonoBehaviour()
    {
        newMonoBehaviours.push_back(this);
    }

    void MonoBehaviour::StartNewMonoBehaviours()
    {
        for(MonoBehaviour* mb : newMonoBehaviours)
        {
            mb->Start();
        }
        newMonoBehaviours.clear();
    }

}