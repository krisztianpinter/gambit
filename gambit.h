#ifndef GAMBIT_H_a5375c8c87ed67d2
#define GAMBIT_H_a5375c8c87ed67d2

/* DISCLAIMER
 * This is NOT a reference implementation!
 * This is a quick and dirty implementation provided to clarify any potential
 * ambiguities or unclear points in the documentation.
 * The software is not thoroughly tested, and not at all tested on platforms
 * other than Intel x86, Windows OS, and CodeBlocks/GNU compiler.
 * You can find the actual documentation at
 * http://docs.google.com/document/d/18R-qEAmL9WWh5zhGeBlvI7C6ikBAz6TF7MEtfPJK7m0
 *
 * This implementation uses Keccak[1600] with c=256 and 512. It does not support
 * the Trans option.
 */

#include <cstddef>

namespace gambit
{
    typedef uint8_t salt [16];

    typedef uint8_t seed128 [32];
    typedef uint8_t dkid128 [168];

    void gambit128(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   const dkid128 dkid, void *key, int key_len);
    void gambit128(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   seed128 seed);
    void gambit128(const seed128 seed, const dkid128 dkid,
                   void *key, int key_len);

    typedef uint8_t seed256 [64];
    typedef uint8_t dkid256 [136];

    void gambit256(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   const dkid256 dkid, void *key, int key_len);
    void gambit256(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   seed256 seed);
    void gambit256(const seed256 seed, const dkid256 dkid,
                   void *key, int key_len);
}

#endif
