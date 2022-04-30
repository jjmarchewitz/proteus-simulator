#include "Component.h"

Component::Component()
{
    Dashboard._Initialize();
    Dashboard.AddComponent(this);
}