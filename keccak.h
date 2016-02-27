#ifndef KECCAK_H_c576330d6876c4b5
#define KECCAK_H_c576330d6876c4b5

#include <stdint.h>

/* DISCLAIMER
 * This implentation of Keccak is not a trusftul implementation.
 * The software is not thoroughly tested, and not at all tested on platforms
 * other than Intel x86, Windows OS, and CodeBlocks/GNU compiler.
 */

namespace keccak
{
    void buffer_xor(void* target, const void* source, int length);

    class keccak_state
    {
        public:
            static const uint64_t round_constants[255];

            uint64_t A[5][5];

            keccak_state();
            ~keccak_state();

            void round(uint64_t rc);

            void f(int rounds = 24);

            void zero(int from_b, int length);

            /* access the state as a buffer. read/write limited to a single
               block, and must not go outside r. */
            void block_absorb(const void *buffer, int from_b, int length);
            void block_squeeze(void *buffer, int from_b, int length);

            // regular 10*1 padding
            void pad101_xor(int from_b, int to_b);

            // custom bbbbbb10*1 padding. b0..b5 will be used
            void pad_custom_101_xor(int from_b, int to_b, uint8_t bits, uint8_t bits_len);

            // direct access to state words
            uint64_t word_read(int idx);
            void word_write_xor(int idx, const uint64_t word);
        protected:
        private:
    };
}

#endif
