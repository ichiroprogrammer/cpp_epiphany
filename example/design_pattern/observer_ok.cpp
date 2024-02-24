#include <iostream>

#include "observer_ok.h"

// @@@ sample begin 0:0

void ObserverOK_0::update(SubjectOK const& subject)
{
    // @@@ ignore begin
    std::cout << "ObserverOK_0:" << subject.GetNum() << std::endl;
    // @@@ ignore end
}

void ObserverOK_1::update(SubjectOK const& subject)
{
    // @@@ ignore begin
    std::cout << "ObserverOK_1:" << subject.GetNum() << std::endl;
    // @@@ ignore end
}

void ObserverOK_2::update(SubjectOK const& subject)
{
    // @@@ ignore begin
    std::cout << "ObserverOK_2:" << subject.GetNum() << std::endl;
    // @@@ ignore end
}
// @@@ sample end
