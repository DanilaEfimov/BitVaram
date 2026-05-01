#include "IntArithmetic.h"
#include "Config.h"
#include <iostream>

int main() {

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Block bb(new byte_t[Config::intSize]{
            std::byte{0x02},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);
        Value b(std::move(bb), bv::Int);

        Value& c = IntArythmetic::add(a, b);

        const auto& bytes = c.bytes().data();
        std::cout << (int)bytes[0] << ' '
            << (int)bytes[1] << ' '
            << (int)bytes[2] << ' '
            << (int)bytes[3] << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x80}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);

        std::cout << IntArythmetic::sign(a) << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);

        IntArythmetic::setsign(a, true);

        const auto& bytes = a.bytes().data();
        std::cout << (int)bytes[0] << ' '
            << (int)bytes[1] << ' '
            << (int)bytes[2] << ' '
            << (int)bytes[3] << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);

        IntArythmetic::negative(a);

        const auto& bytes = a.bytes().data();
        std::cout << (int)bytes[0] << ' '
            << (int)bytes[1] << ' '
            << (int)bytes[2] << ' '
            << (int)bytes[3] << std::endl;

        IntArythmetic::positive(a);

        const auto& bytes2 = a.bytes().data();
        std::cout << (int)bytes2[0] << ' '
            << (int)bytes2[1] << ' '
            << (int)bytes2[2] << ' '
            << (int)bytes2[3] << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Block bb = ba;

        Value a(std::move(ba), bv::Int);
        Value b(std::move(bb), bv::Int);

        std::cout << IntArythmetic::eq(a, b) << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Block bb(new byte_t[Config::intSize]{
            std::byte{0x02},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);
        Value b(std::move(bb), bv::Int);

        std::cout << IntArythmetic::le(a, b) << std::endl;
        std::cout << IntArythmetic::ge(b, a) << std::endl;
    }

    {
        Block ba(new byte_t[Config::intSize]{
            std::byte{0x01},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x00}
            }, Config::intSize);

        Block bb(new byte_t[Config::intSize]{
            std::byte{0x02},
            std::byte{0x00},
            std::byte{0x00},
            std::byte{0x80}
            }, Config::intSize);

        Value a(std::move(ba), bv::Int);
        Value b(std::move(bb), bv::Int);

        std::cout << IntArythmetic::le_abs(a, b) << std::endl;
    }

    return 0;
}