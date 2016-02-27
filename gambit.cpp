#include "keccak.h"
#include "gambit.h"
#include <cassert>
#include <cstring>

#define KECCAK_BACKWARD_RATIO 100
// todo: 100 is not an actual value. actual value has to be found.

namespace gambit
{
    using namespace keccak;

    unsigned int gcd(unsigned int a, unsigned int b)
    {
        while (true)
        {
            a %= b;
            if (a == 0) return b;
            b %= a;
            if (b == 0) return a;
        }
    }

    void gambit(unsigned int r,
                const void *salt,
                const char* pwd, unsigned int pwd_len,
                const uint64_t* ROM, unsigned int ROM_len,
                unsigned int cost_t, unsigned int cost_m,
                unsigned int p,
                void *seed)
    {
        assert (cost_m & 1);
        assert (cost_t > 0);
        assert (cost_m*2 <= cost_t * (r/8) );
        assert (pwd_len+16+1 <= r);
        assert (p > 0);

        uint64_t* mem = new uint64_t[cost_m];
        memset(mem, 0, sizeof(uint64_t)*cost_m);

        unsigned int f;
        if (cost_m == 1)
            f = 1;
        else
        {
            f = cost_m * KECCAK_BACKWARD_RATIO / (KECCAK_BACKWARD_RATIO + 1);
            while ( (gcd(cost_m, f) != 1) || (gcd(cost_m, f - 1) != 1) ) f--;
        }

        keccak_state A[p];
        for (unsigned int i = 0; i < p; i++)
        {
            A[i].block_absorb(salt, 0, 16);
            A[i].block_absorb(pwd, 16, pwd_len);
            A[i].pad_custom_101_xor(16 + pwd_len, r-1, i & 0x3F, 6);
            A[i].f();
            A[i].zero(0, r);
            A[i].f();
        }

        unsigned int wrtp = 0;
        unsigned int rdp = 0;
        unsigned int romp = 0;

        for (; cost_t > 0; cost_t--)
        {
            for (unsigned int i = 0; i < r/8; i++)
            {
                for (unsigned lane = 0; lane < p; lane++)
                {
                    mem[wrtp] ^= A[lane].word_read(i);
                    wrtp++;
                    if (wrtp == cost_m) wrtp = 0;

                    A[lane].word_write_xor(i, mem[rdp] ^ ROM[romp]); // todo: xor? depends on sse opcodes
                    rdp += f;
                    if (rdp >= cost_m) rdp -= cost_m;
                    romp++;
                    if (romp >= ROM_len) romp = 0;
                }
            }
            for (unsigned lane = 0; lane < p; lane++)
                A[lane].f();
        }

        memset(mem, 0, sizeof(uint64_t)*cost_m);
        delete [] mem;

        memset(seed, 0, 200-r);
        for (unsigned lane = 0; lane < p; lane++)
        {
            char tmp[200-r];
            A[lane].block_squeeze(tmp, r, 200-r);
            buffer_xor(seed, tmp, 200-r);
        }
    }

    // // // // // // // // 256 // // // // // // // //

    void gambit128(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   unsigned int p,
                   seed128 seed)
    {
        gambit(168, salt, pwd, pwd_len, ROM, ROM_len, cost_t, cost_m, p, seed);
    }

    void gambit128(const seed128 seed, const dkid128 dkid, void *key, int key_len)
    {
        assert (key_len <= 168);

        keccak_state A;
        A.block_absorb(dkid, 0, 168);
        A.block_absorb(seed, 168, 32);
        A.f();
        A.block_squeeze(key, 0, key_len);
    }

    void gambit128(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   unsigned int p,
                   const dkid128 dkid, void *key, int key_len)
    {
        seed128 seed;
        gambit128(salt, pwd, pwd_len, ROM, ROM_len, cost_t, cost_m, p, seed);
        gambit128(seed, dkid, key, key_len);
        memset(seed, 0, 32);
    }

    // // // // // // // // 512 // // // // // // // //

    void gambit256(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   unsigned int p,
                   seed256 seed)
    {
        gambit(136, salt, pwd, pwd_len, ROM, ROM_len, cost_t, cost_m, p, seed);
    }

    void gambit256(const seed256 seed, const dkid256 dkid, void *key, int key_len)
    {
        assert (key_len <= 136);

        keccak_state A;
        A.block_absorb(dkid, 0, 136);
        A.block_absorb(seed, 136, 64);
        A.f();
        A.block_squeeze(key, 0, key_len);
    }

    void gambit256(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   unsigned int p,
                   const dkid256 dkid, void *key, int key_len)
    {
        seed256 seed;
        gambit256(salt, pwd, pwd_len, ROM, ROM_len, cost_t, cost_m, p, seed);
        gambit256(seed, dkid, key, key_len);
        memset(seed, 0, 64);
    }
}
