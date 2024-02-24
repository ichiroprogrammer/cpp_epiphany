#include "subject_ok.h"

// @@@ sample begin 0:0

void SubjectOK::Attach(Observer& observer_to_attach) { observers_.push_back(&observer_to_attach); }

void SubjectOK::Detach(Observer& observer_to_detach) noexcept
{
    observers_.remove_if(
        [&observer_to_detach](Observer* observer) { return &observer_to_detach == observer; });
}

void SubjectOK::notify() const
{
    for (auto observer : observers_) {
        observer->Update(*this);
    }
}
// @@@ sample end
