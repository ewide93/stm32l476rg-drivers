import ctypes
import logging
from pathlib import Path


logger = logging.getLogger(__name__)


class Result(ctypes.Structure):
    _fields_ = [("length", ctypes.c_uint16), ("valid", ctypes.c_bool)]


class CobsEncodeError(Exception):
    pass


class CobsDecodeError(Exception):
    pass


class CobsCodec:
    def __init__(
            self,
            dll_path: Path,
            max_buffer_size: int = 1024,
            *,
            debug: bool
        ) -> None:

        self._dll = ctypes.cdll.LoadLibrary(str(dll_path))
        self._dll.CobsCodec_Encode.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.c_uint16,
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.c_uint16
        ]
        self._dll.CobsCodec_Encode.restype = Result
        self._dll.CobsCodec_Decode.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.c_uint16,
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.c_uint16
        ]
        self._dll.CobsCodec_Decode.restype = Result
        self._max_buffer_size = max_buffer_size
        logging.basicConfig(level=logging.DEBUG if debug else logging.INFO)

    def encode(self, data: bytearray) -> bytearray:
        src = (ctypes.c_uint8 * len(data)).from_buffer(data)
        encode_buffer = (ctypes.c_uint8 * self._max_buffer_size).from_buffer(bytearray(self._max_buffer_size))
        result = self._dll.CobsCodec_Encode(
            src,
            ctypes.c_uint16(len(src)),
            encode_buffer,
            ctypes.c_uint16(self._max_buffer_size)
        )

        if not result.valid:
            raise CobsEncodeError from None
        logger.debug(f"Raw data: {[hex(byte) for byte in data]}")
        logger.debug(f"Encoded data: {[hex(byte) for byte in encode_buffer[:result.length]]}")
        return bytearray(encode_buffer[:result.length])

    def decode(self, data: bytearray) -> bytearray:
        src = (ctypes.c_uint8 * len(data)).from_buffer(data)
        decode_buffer = (ctypes.c_uint8 * self._max_buffer_size).from_buffer(bytearray(self._max_buffer_size))
        result = self._dll.CobsCodec_Decode(
            src,
            ctypes.c_uint16(len(src)),
            decode_buffer,
            ctypes.c_uint16(self._max_buffer_size)
        )

        if not result.valid:
            raise CobsDecodeError from None
        logger.debug(f"Encoded data: {[hex(byte) for byte in data]}")
        logger.debug(f"Decoded data: {[hex(byte) for byte in decode_buffer[:result.length]]}")

        return bytearray(decode_buffer[:result.length])


if __name__ == "__main__":
    codec = CobsCodec(Path("stm32l476rg/build/dlls/cobs_codec"), debug=True)
    x = codec.encode(bytearray([0x00]))
    y = codec.decode(x)

    a = codec.encode(bytearray([0x00, 0x00]))
    b = codec.decode(a)

    c = codec.encode(bytearray([0x11, 0x22, 0x33, 0x44]))
    d = codec.decode(c)
