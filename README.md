# nexbus-core

`nexbus-core` is a portable C++17 library implementing Nexbus v0.1 frame encoding, decoding, CRC, and payload helpers. It has no MCU SDK dependency and is intended for shared use across RP2040, ESP32, and desktop tools.

## Framing (Nexbus v0.1)

Frame bytes:

- `SOF` = `0xA5`
- `VER` = `0x01`
- `LEN` (`u16`, little-endian)
- `SRC` (`u8`)
- `DST` (`u8`) (`0x00` master, `0xFF` broadcast)
- `MSG` (`u8`)
- `SEQ` (`u8`)
- `FLAGS` (`u8`)
- `PAYLOAD` (`LEN` bytes)
- `CRC16` (`u16`, little-endian)

CRC is CRC-16/CCITT-FALSE over `VER..PAYLOAD` (SOF excluded).

## Usage

### Encode a frame

```cpp
nexbus::Frame f{};
f.ver = nexbus::kVersion;
f.src = 0x10;
f.dst = 0x00;
f.msg = nexbus::msg::PING;
f.seq = 1;
f.flags = 0;
f.payload = {payload_ptr, payload_len};
f.len = static_cast<uint16_t>(f.payload.size);

uint8_t out[128];
size_t n = nexbus::encode_frame(f, out, sizeof(out));
```

### Parse bytes incrementally

```cpp
nexbus::Parser parser;
for (uint8_t b : stream) {
  if (parser.push(b)) {
    const nexbus::Frame& frame = parser.frame();
    // consume frame
  }
}
```

## Tests

```bash
pio test -e native
```

## Versioning

Tag releases as `v0.1.x` for Nexbus protocol v0.1-compatible changes.
