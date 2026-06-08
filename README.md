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
  {"todo": 1}
]
```
### C Header
```C
// TODO
```

## Dependencies
- Boost v1.91.0
