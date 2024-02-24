#include <iostream>
#include <optional>

#include "gtest_wrapper.h"

#include "observer_ng.h"
#include "observer_ok.h"
#include "subject_ng.h"
#include "subject_ok.h"

namespace {

TEST(Observer, observer_ng)
{
    // @@@ sample begin 0:0

    struct ObserverNG_0_Test : ObserverNG_0 {  // テスト用クラス
        virtual void Update(SubjectNG const& subject) final
        {
            ++call_count;
            num = subject.GetNum();
        }

        uint32_t                call_count{0};
        std::optional<uint32_t> num{};
    };

    auto ng0 = ObserverNG_0_Test{};
    auto ng1 = ObserverNG_1{};
    auto ng2 = ObserverNG_2{};

    auto subject = SubjectNG{ng0, ng1, ng2};

    ASSERT_EQ(0, ng0.call_count);  // まだ何もしていない
    ASSERT_FALSE(ng0.num);

    subject.SetNum(1);
    subject.SetNum(2);

    ASSERT_EQ(2, ng0.call_count);
    ASSERT_EQ(2, *ng0.num);

    subject.SetNum(2);  // 同じ値をセットしたため、Updateは呼ばれないはず
    ASSERT_EQ(2, ng0.call_count);
    ASSERT_EQ(2, *ng0.num);
    // @@@ sample end
}

TEST(Observer, observer_ok)
{
    // @@@ sample begin 0:1

    struct ObserverOK_Test : Observer {  // テスト用クラス
        virtual void update(SubjectOK const& subject) final
        {
            ++call_count;
            num = subject.GetNum();
        }

        uint32_t                call_count{0};
        std::optional<uint32_t> num{};
    };

    auto ok0 = ObserverOK_Test{};
    auto ok1 = ObserverOK_1{};
    auto ok2 = ObserverOK_2{};

    auto subject = SubjectOK{};

    subject.Attach(ok0);
    subject.Attach(ok1);
    subject.Attach(ok2);

    ASSERT_EQ(0, ok0.call_count);  // まだ何もしていない
    ASSERT_FALSE(ok0.num);

    subject.SetNum(1);
    subject.SetNum(2);

    ASSERT_EQ(2, ok0.call_count);
    ASSERT_EQ(2, *ok0.num);

    subject.SetNum(2);  // 同じ値をセットしたため、Updateは呼ばれないはず
    ASSERT_EQ(2, ok0.call_count);
    ASSERT_EQ(2, *ok0.num);
    // @@@ sample end

    auto ok3 = ObserverOK_Test{};

    subject.Attach(ok3);
    subject.Detach(ok0);

    subject.SetNum(3);
    ASSERT_EQ(2, ok0.call_count);
    ASSERT_EQ(2, *ok0.num);
    ASSERT_EQ(1, ok3.call_count);
    ASSERT_EQ(3, *ok3.num);
}
}  // namespace
