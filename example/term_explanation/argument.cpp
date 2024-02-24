namespace ArgumentSample {
// @@@ sample begin 0:0

int f0(int a, int& b) noexcept  // a, bは仮引数
{
    // @@@ ignore begin
    b = 3;
    return a + 1;
    // @@@ ignore end
}

void f1() noexcept
{
    // @@@ ignore begin
    auto x = 3;
    auto y = 0;
    // @@@ ignore end

    f0(x, y);  // x, yは実引数
}
// @@@ sample end
}  // namespace ArgumentSample
