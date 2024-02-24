#include <iostream>

#include "observer_ng.h"
#include "subject_ng.h"

// @@@ sample begin 0:0

void ObserverNG_1::Update(SubjectNG const& subject)
{
    // @@@ ignore begin
    std::cout << "ObserverNG_1:" << subject.GetNum() << std::endl;
    // @@@ ignore end
}

void ObserverNG_2::Update(SubjectNG const& subject)
{
    // @@@ ignore begin
    std::cout << "ObserverNG_2:" << subject.GetNum() << std::endl;
    // @@@ ignore end
}
// @@@ sample end
