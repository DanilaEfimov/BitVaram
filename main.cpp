#include <iostream>
#include "Int32.h"
#include "Int64.h"
#include "Uint32.h"
#include "Text.h"
#include "Array.h"
#include "RawBytes.h"
#include "Bit.h"

void demo_int32() {
    std::cout << "=== Int32 ===\n";
    Int32 a(10), b(3);

    std::cout << "a = " << a.get() << "\n";
    std::cout << "b = " << b.get() << "\n";
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * b = " << (a * b) << "\n";
    std::cout << "a / b = " << (a / b) << "\n";
    std::cout << "a > b = " << (a > b) << "\n";
    std::cout << "a < b = " << (a < b) << "\n";
    std::cout << "a == b = " << (a == b.get()) << "\n";

    Int32 neg(-42);
    std::cout << "neg = " << neg.get() << "\n";
    std::cout << "neg.sign() = " << neg.sign() << "\n";
    std::cout << "neg.abs()  = " << neg.abs() << "\n";
}

void demo_int64() {
    std::cout << "\n=== Int64 ===\n";
    Int64 big(9000000000LL), small(1000000000LL);

    std::cout << "big + small = " << (big + small) << "\n";
    std::cout << "big * small = " << (big * small) << "\n";

    Int64 neg(-99);
    std::cout << "sign(-99) = " << neg.sign() << "\n";
    std::cout << "abs(-99)  = " << neg.abs() << "\n";
}

void demo_uint32() {
    std::cout << "\n=== Uint32 ===\n";
    Uint32 x(100), y(7);

    std::cout << "x = " << x.get() << "\n";
    std::cout << "x + y = " << (x + y) << "\n";
    std::cout << "x - y = " << (x - y) << "\n";
    std::cout << "x / y = " << (x / y) << "\n";
    std::cout << "x > y = " << (x > y) << "\n";

    // set
    x.set(42);
    std::cout << "after set(42): x = " << x.get() << "\n";
}

void demo_text() {
    std::cout << "\n=== Text ===\n";
    Text t1("Hello"), t2(", world!");
    Text t3 = t1 + t2;

    std::cout << t3.get() << "\n";
    std::cout << "t1 == \"Hello\" : " << (t1 == "Hello") << "\n";
    std::cout << "t1 == \"Bye\"   : " << (t1 == "Bye") << "\n";

    t1.set("Changed");
    std::cout << "after set: " << t1.get() << "\n";
}

void demo_array() {
    std::cout << "\n=== Array<int> ===\n";
    Array<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);

    std::cout << "size = " << arr.size() << "\n";
    for (size_t i = 0; i < arr.size(); i++)
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";

    arr.remove(1);
    std::cout << "after remove(1), size = " << arr.size() << "\n";
    std::cout << "arr[0]=" << arr[0] << " arr[1]=" << arr[1] << "\n";

    // out_of_range
    try {
        auto _ = arr[2];
    }
    catch (const std::out_of_range& e) {
        std::cout << "out_of_range: " << e.what() << "\n";
    }
}

void demo_bit() {
    std::cout << "\n=== Bit ===\n";
    Bit::setByteLineSize(16);

    Bit::write(0, true);
    Bit::write(1, false);
    Bit::write(2, true);

    std::cout << "bit[0] = " << Bit::read(0) << "\n";
    std::cout << "bit[1] = " << Bit::read(1) << "\n";
    std::cout << "bit[2] = " << Bit::read(2) << "\n";

    Bit b(3, true);
    std::cout << "b.get() = " << b.get() << "\n";
    b.set(false);
    std::cout << "after set(false): b.get() = " << b.get() << "\n";
}

void demo_raw_bytes() {
    std::cout << "\n=== RawBytes ===\n";

    RawBytes rb;

    rb.set(0, 0xFF);
    rb.set(1, 0xAB);
    rb.set(2, 0x00);
    rb.set(3, 0x42);

    std::cout << std::hex << std::uppercase;
    std::cout << "rb[0] = 0x" << (int)rb[0] << "\n";  // FF
    std::cout << "rb[1] = 0x" << (int)rb[1] << "\n";  // AB
    std::cout << "rb[2] = 0x" << (int)rb[2] << "\n";  // 00
    std::cout << "rb[3] = 0x" << (int)rb[3] << "\n";  // 42

    RawBytes raw{};
    raw.set(0, 0xDE);
    raw.set(1, 0xAD);
    raw.set(2, 0xBE);
    raw.set(3, 0xEF);
    rb.set(raw.get());

    std::cout << "after set(DEADBEEF...):\n";
    auto val = rb.get();
    for (size_t i = 0; i < rawBytesSize; i++)
        std::cout << "rb[" << i << "] = 0x" << (int)val.read(i) << "\n";

    RawBytes rb2;
    rb2.set(raw.get());
    std::cout << "rb == rb2: " << (rb == rb2.get()) << "\n";  // 1

    rb2.set(0, 0x00);
    std::cout << "rb == rb2 (after modify): " << (rb == rb2.get()) << "\n";  // 0

    std::cout << std::dec;
}

int main() {
    demo_int32();
    demo_int64();
    demo_uint32();
    demo_text();
    demo_array();
    demo_bit();
    demo_raw_bytes();
    return 0;
}