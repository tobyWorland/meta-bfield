# meta-bfield

Takes a JSON spec detailing all the parts in a binary field and will generate functions for:
- Matching reserved/constant parts
- Encoding the binary field from a struct of the parts
- Decoding the binary field by extracting all the parts out into a struct

Written for pragmatically generating encoding and decoding functions for ARM's thumb instruction set, for use in the assembler and disassembler in [tw-monitor](https://github.com/tobyWorland/tw-monitor)

## Example
### Spec
```JSON
[
    {
        "name": "adds_i_t1",
        "width": 16,
        "parts": [
            "000 11 1 0",
            {"name": "imm3", "width": 3},
            {"name": "Rn", "width": 3},
            {"name": "Rd", "width": 3}
        ]
    },
    {
        "name": "adds_i_t2",
        "width": 16,
        "parts": [
            "001 10",
            {"name": "Rdn", "width": 3},
            {"name": "imm8", "width": 8}
        ]
    },
    {
        "name": "addw_i_t4",
        "width": 32,
        "swapped": true,
        "parts": [
            "11110",
            {"name": "i", "width": 1},
            "1 0000 0",
            {"name": "Rn", "width": 4},

            "0",
            {"name": "imm3", "width": 3},
            {"name": "Rd", "width": 4},
            {"name": "imm8", "width": 8}
        ],
        "export": [
            "Rd",
            "Rn",
            {"name": "imm12", "parts": ["i", "imm3", "imm8"], "signed": false}
        ]
    }
]
```
### C Header
```C
// AUTOGEN - DO NOT EDIT

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct adds_i_t1_parts {
    uint32_t imm3;
    uint32_t Rn;
    uint32_t Rd;
};

struct adds_i_t2_parts {
    uint32_t Rdn;
    uint32_t imm8;
};

struct addw_i_t4_parts {
    uint32_t Rd;
    uint32_t Rn;
    uint32_t imm12;
};

bool match_adds_i_t1(uint16_t field);
bool match_adds_i_t2(uint16_t field);
bool match_addw_i_t4(uint32_t field);

unsigned encode_adds_i_t1(uint16_t *out, struct adds_i_t1_parts *parts);
unsigned encode_adds_i_t2(uint16_t *out, struct adds_i_t2_parts *parts);
unsigned encode_addw_i_t4(uint32_t *out, struct addw_i_t4_parts *parts);

struct adds_i_t1_parts decode_adds_i_t1(uint16_t field);
struct adds_i_t2_parts decode_adds_i_t2(uint16_t field);
struct addw_i_t4_parts decode_addw_i_t4(uint32_t field);
```

## Dependencies
- Boost v1.91.0
