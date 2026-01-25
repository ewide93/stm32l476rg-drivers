import os
import sys
import time
import struct
from serial.tools import list_ports
from crc import Calculator, Crc8
from serial import Serial
from typing import Optional


def find_usb_device(match_str: str) -> Optional[str]:
    device_path: Optional[str] = None
    devices = list_ports.comports()
    for device in devices:
        if match_str in device.description:
            device_path = f"/dev/{device.name}"
            break
    return device_path

if __name__ == "__main__":

    # calc = Calculator(Crc8.SAEJ1850.value)
    # data = bytearray([0x00])
    # data.extend(bytearray(8))
    # crc = calc.checksum(data)
    # data.append(crc & 0xFF)
    # dev = Serial(com_port, baudrate=115200)
    # settings = dev.get_settings()
    # settings["timeout"] = 1.0
    # dev.apply_settings(settings)

    # print(f"Sending data: {[hex(byte) for byte in data]}")
    # t0 = time.perf_counter()
    # dev.write(data)
    # resp = dev.read(10)
    # t1 = time.perf_counter()
    # print(f"Message exchange took {round((t1 - t0) * 1e3, 3)} ms")
    # ret_code = struct.unpack_from("<B", resp, 0)[0]
    # pl1 = struct.unpack_from("<L", resp, 1)[0]
    # pl2 = struct.unpack_from("<L", resp, 5)[0]
    # recv_crc = struct.unpack_from("<B", resp, 9)[0]
    # print(f"Retval: {hex(ret_code)}, Pl1: {hex(pl1)}, Pl2: {hex(pl2)}, CRC: {recv_crc} Calc CRC: {calc.checksum(resp[:9])}")
    # print(f"Uptime: {pl1} [RTOS ticks]")
    # print(f"RTOS ticks per second: {pl2} [RTOS ticks]")
    # time.sleep(0.1)
    # dev.close()


    com_port = find_usb_device("STM32")
    calc = Calculator(Crc8.SAEJ1850.value)
    data = bytearray([0x01])
    data.extend(bytearray(8))
    crc = calc.checksum(data)
    data.append(crc & 0xFF)
    dev = Serial(com_port, baudrate=115200)
    settings = dev.get_settings()
    settings["timeout"] = 1.0
    dev.apply_settings(settings)

    print(f"Sending data: {[hex(byte) for byte in data]}")
    t0 = time.perf_counter()
    dev.write(data)
    resp = dev.read(10)
    t1 = time.perf_counter()
    print(f"Message exchange took {round((t1 - t0) * 1e3, 3)} ms")
    ret_code = struct.unpack_from("<B", resp, 0)[0]
    pl1 = struct.unpack_from("<B", resp, 1)[0]
    pl2 = struct.unpack_from("<B", resp, 2)[0]
    recv_crc = struct.unpack_from("<B", resp, 9)[0]
    print(f"Retval: {hex(ret_code)}, Pl1: {hex(pl1)}, Pl2: {hex(pl2)}, CRC: {recv_crc} Calc CRC: {calc.checksum(resp[:9])}")
    time.sleep(0.1)
    dev.close()

