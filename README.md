# Gambit high cost PBKDF

## legal notes

This work is in the public domain. no rights reserved.

If you want to blame anyone, you can blame:
Krisztián Pintér, pinterkr@gmail.com 

This is NOT a reference implementation!

This is a quick and dirty implementation. The software is not thoroughly
tested, and not at all tested on platforms other than Intel x86,
Windows OS, and CodeBlocks/GCC. Once tested and verified, the
purpose of this program is to print test vectors to stdout that can
be compared to the output of another implementation. Also it serves as
additional documentation, to compelemnt the formal definition.


## formal definition

https://docs.google.com/document/d/18R-qEAmL9WWh5zhGeBlvI7C6ikBAz6TF7MEtfPJK7m0


## interface

Gambit has two security levels in this implementation, 128 and 256 bit security,
that is, 256 and 512 bit capacity. The name refers to the security level.

For both security levels, there are three main interface functions. One takes
the password and the salt, and generates the derivation seed. The second takes
this derivation seed, and a derivation key id, and returns the derived key.
There is a third one that combines the above two, skipping the derivation key.

The exact definitions are:

    void gambit###(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   seed### seed);
    void gambit###(const seed### seed, const dkid### dkid,
                   void *key, int key_len);
    void gambit###(const salt salt,
                   const char* pwd, unsigned int pwd_len,
                   const uint64_t* ROM, unsigned int ROM_len,
                   unsigned int cost_t, unsigned int cost_m,
                   const dkid### dkid, void *key, int key_len);

The ### is the choosen security level, 128 or 256. The other parameters
correspond to Gambit parameters as defined in the documentation. The pwd
parameter is not null terminated, pwd_len characters will be used. The pwd
characters will be treated as binary, no conversion of any sort is applied.


## PHC version

Please note that this version is different than the one submitted to the
Password Hashing Competition. The major changes are:

* the PHS interface functions are removed.
* an additional keccak-f round is added before the stretching. See the def.
* the interface functions are renamed to indicate security, not capacity.
