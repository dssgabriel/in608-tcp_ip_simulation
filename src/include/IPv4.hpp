#pragma once

#include <bitset>
#include <iostream>

struct IPv4 {
    std::bitset<8> a;
    std::bitset<8> b;
    std::bitset<8> c;
    std::bitset<8> d;

    bool operator<(const IPv4& ip) const {
        return (
              (a.to_ulong() < ip.a.to_ulong())
            & (b.to_ulong() < ip.b.to_ulong())
            & (c.to_ulong() < ip.c.to_ulong())
            & (d.to_ulong() < ip.d.to_ulong())
        );
    }

    bool operator==(const IPv4& ip) const {
        return (
              (a.to_ulong() == ip.a.to_ulong())
            & (b.to_ulong() == ip.b.to_ulong())
            & (c.to_ulong() == ip.c.to_ulong())
            & (d.to_ulong() == ip.d.to_ulong())
        );
    }

    friend std::ostream& operator<<(
        std::ostream& flux, 
        const IPv4& ip
    ) {
        flux << ip.a.to_ulong() << "." << ip.b.to_ulong() << ".";
        flux << ip.c.to_ulong() << "." << ip.d.to_ulong();

        return flux;
    }
};

extern IPv4 ipZero;
extern IPv4 ipMax;
