#include "observer_ng.h"
#include "subject_ng.h"

// @@@ sample begin 0:0

void SubjectNG::SetNum(uint32_t num)
{
    if (num_ == num) {
        return;
    }

    num_ = num;

    notify();  // subjectが変更されたことをobserverへ通知
}

void SubjectNG::notify()
{
    ng_0_.Update(*this);
    ng_1_.Update(*this);
    ng_2_.Update(*this);
}
// @@@ sample end
