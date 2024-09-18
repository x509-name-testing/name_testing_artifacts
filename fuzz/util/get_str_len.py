from itertools import islice

def get_length(val: str) -> bytes:
    lenlen = 0 # the byte length of ‘L’
    real_len = len(val)
    if real_len:
        if real_len > 127:
            lenlen = 1
        while real_len > 255 :
            lenlen += 1
            real_len = real_len >> 8
    
    # import pdb; pdb.set_trace()

    if lenlen == 1:
        ret_len = int_to_bytes(0x80 + lenlen) + int_to_bytes(real_len)
    elif lenlen > 1:
        ret_len = int_to_bytes(0x80 + lenlen) + int_to_bytes2(len(val))
    else:
        ret_len = int_to_bytes(real_len)
    return ret_len


def int_to_bytes2(x: int) -> bytes:
    xx = hex(x)[2:]
    # convert xx to bytes
    if len(xx) % 2 == 1:
        xx = '0' + xx
    return bytes.fromhex(xx)

def int_to_bytes(x: int) -> bytes:
    return x.to_bytes((x.bit_length() + 7) // 8, 'big')
    
def int_from_bytes(xbytes: bytes) -> int:
    return int.from_bytes(xbytes, 'big')



TAG_FIELD_LENGTH = 1
def tlv_parser(tlv_string):
    it = iter(tlv_string)
    try:
        while tag := next(islice(it, TAG_FIELD_LENGTH)):
            # indefinite length
            # check if the MSB of the length field is 1
            if ( (length := next(islice(it, 1)) ) & 0x80):
                # get the number of length bytes
                length_bytes = length & 0x7F
                length = int.from_bytes(bytes(islice(it, length_bytes)),'big')
            # print(tag, length)
            # value = bytes(islice(it, length))
            # value = bytes(islice(it, length)).decode()
            value = bytes(islice(it, length))
            # value = tlv_string[1+]
            yield (tag, int_to_bytes(length), value)
    except StopIteration as e:
        # print(e)
        pass


if __name__ == "__main__":
    # # tlv = bytes.fromhex("1388" + "7fffffffffffffff"+"00"*0x7fffffffffffffff)
    # tlv = b'\x1c\x81\xd7a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
    tlv = b'\x82\x06\xf4\x84\x98\x8c./'
    # print(tlv)
    import sys; print(hex(sys.maxsize))
    for tag, length, value in tlv_parser(tlv):
        print(tag, length, value)
    # print(get_length("a"*257))