import unicodedata as ucd
from math import ceil
import random
# import string

'''
reference: http://www.unicode.org/Public/UNIDATA/Blocks.txt
reference: http://www.unicode.org/Public/UNIDATA/Scripts.txt
reference: https://www.rfc-editor.org/rfc/rfc3454
space_lst: a list of all the space characters in the Unicode database
control_lst: a list of all the control characters in the Unicode database
'''

def list_diff(lst1, lst2):
    return list(set(lst1) - set(lst2))

ascii_space_lst = [9, 10, 11, 12, 13, 32]
ascii_control_lst = [i for i in range(0, 32)]
ascii_symbol_lst = [i for i in range(33, 48)] + [i for i in range(58, 65)] + [i for i in range(91, 97)] + [i for i in range(123, 127)]
ascii_alphanum_lst = [i for i in range(48, 58)] + [i for i in range(65, 91)] + [i for i in range(97, 123)]
ascii_char_lst = set(ascii_control_lst + ascii_space_lst + ascii_symbol_lst + ascii_alphanum_lst)

ia5_lst = list(set(ascii_char_lst) - set([35, 36, 64, 91, 92, 93, 94, 96, 123, 124, 125, 126]))
printable_lst = ascii_alphanum_lst + [32, 58, 61, 63] + [i for i in range(39, 48)]

bmp_lst = [i for i in range(0, 0x10000)]
octet_lst = [i for i in range(0, 256)]
graphic_lst = [i for i in range(33, 127)]

teletex_lst = []
universal_lst = []
general_lst = []
numeric_lst = [i for i in range(48, 58)]
videotex_lst = []
visible_lst = printable_lst + [i for i in range(160, 256)]


### reference: https://www.rfc-editor.org/rfc/rfc3454

## section 3.1 map to nothing
map_to_nothing_lst = [0xad, 0x34f, 0x1806] + list(range(0x180b, 0x180d+1)) \
                + list(range(0x200b, 0x200d+1)) + [0x2060] + list(range(0xfe00, 0xfe0f+1))

## section 3.2 case folding
# case_folding_lst

white_space_lst = [9, 10, 11, 12, 13, 32]
## section 5.1 space characters
space_lst = [ 0x20, 0xA0, 0x2000, 0x2001, 0x2002, 0x2003,
                0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 
                0x200A, 0x200B, 0x3000, 0xFEFF ]
## section 5.2 control characters
control_lst = list(range(0, 0x1f+1)) + list(range(0x7f, 0x9f+1)) \
                + [0x6dd, 0x70f, 0x180e, 0x200c, 0x200d, 0x2028, 0x2029, 
                0x2060, 0x2061, 0x2062, 0x2063, 0xfeff] + list(range(0x206a, 0x206f+1)) \
                + list(range(0x1ff9, 0x1ffc+1)) + list(range(0x1d173, 0x1d17a+1))
## section 5.3 non-printing characters
private_use_lst = list(range(0xe000, 0xf8ff+1)) + list(range(0xf0000, 0x1fffd+1)) \
                + list(range(0x100000, 0x10fffd+1))
## section 5.4 Non-character code points
non_char_point_lst = list(range(0xfdd0, 0xfdef+1)) + list(range(0x1ffe, 0x1fff+1))  \
                + list(range(0x1fffe, 0x1ffff+1)) + list(range(0x2fffe, 0x2ffff+1)) \
                + list(range(0x3fffe, 0x3ffff+1)) + list(range(0x4fffe, 0x4ffff+1)) \
                + list(range(0x5fffe, 0x5ffff+1)) + list(range(0x6fffe, 0x6ffff+1)) \
                + list(range(0x7fffe, 0x7ffff+1)) + list(range(0x8fffe, 0x8ffff+1)) \
                + list(range(0x9fffe, 0x9ffff+1)) + list(range(0xafffe, 0xaffff+1)) \
                + list(range(0xbfffe, 0xbffff+1)) + list(range(0xcfffe, 0xcffff+1)) \
                + list(range(0xdfffe, 0xdffff+1)) + list(range(0xefffe, 0xeffff+1)) \
                + list(range(0x1fffe, 0x1ffff+1)) + list(range(0x10fffe, 0x10ffff+1))
## section 5.5 Surrogates
surrogate_lst = list(range(0xd800, 0xdfff+1))
## section 5.6 Inappropriate for plain text
inappropriate_for_plain_text_lst = [0x7f, 0x80, 0x81, 0x8d, 0x90, 0x9d, 0xa0, 0xad, 0x600, 0x601, 0x6dd, 0x70f, 0x180e, 0x200c, 0x200d, 0x2028, 0x2029, 0x2060, 0x2061, 0x2062, 0x2063, 0xfeff] + list(range(0x206a, 0x206f+1)) \
                + list(range(0x1ff9, 0x1ffc+1)) + list(range(0x1d173, 0x1d17a+1))
## section 5.7 Inappropriate for canonical representation
inappropriate_for_canonical_replacement_lst = [0x2ff0, 0x2ff1, 0x2ff2, 0x2ff3, 0x2ff4, 0x2ff5, 0x2ff6, 0x2ff7, 0x2ff8, 0x2ff9, 0x2ffa, 0x2ffb,]
## section 5.8 Change display properties or are deprecated
change_display_properties_lst = [0x0340, 0x0341, 0x200e, 0x200f, 0x202a, 0x202b, 0x202c, 0x202d, 0x202e, 0x206a, 0x206b, 0x206c, 0x206d, 0x206e, 0x206f,]
## section 5.9 Tagging characters
tagging_character_lst = [0xe0001,] + list(range(0xe0020, 0xe007f+1))
## summary of section 5
prohibit_char_lst = map_to_nothing_lst + control_lst + private_use_lst + non_char_point_lst \
                + surrogate_lst + inappropriate_for_plain_text_lst \
                + inappropriate_for_canonical_replacement_lst + change_display_properties_lst \
                + tagging_character_lst

## section 6.1 Characters with bidirectional property R or AL
# bidirectional_char_lst = 

## section 7 unasigned code points
# unassigned_code_point_lst = 

utf8_lst = list_diff([i for i in range(127, 0x10FFFF)], prohibit_char_lst)


'''
get_asc_space(): return a ascii space
get_ucd_space(): return a unicode space
'''
def get_asc_space():
    sample = random.sample(white_space_lst, min(ceil(0.1*len(white_space_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

# def get_all_ucd_space():
#     return [chr(i) for i in space_lst]

def get_ucd_space():
    sample = random.sample(space_lst, min(ceil(0.1*len(space_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_asc_control():
    llst = list(range(0x00, 0x1f+1)) + [0x7f]
    sample = random.sample(llst, min(ceil(0.1*len(llst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_ucd_control():
    sample = random.sample(control_lst, min(ceil(0.1*len(control_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_asc_char():
    llst = list(range(0, 127))
    sample = random.sample(llst, min(ceil(0.1*len(llst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_ucd_char():
    sample = random.sample(utf8_lst, min(ceil(0.1*len(utf8_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_non_char():
    sample = random.sample(non_char_point_lst, min(ceil(0.1*len(non_char_point_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_null_char():
    # return [chr(0)]
    return [bytes(chr(0), "utf-8")]

def get_asc_prohibit():
    union = list(set(ascii_char_lst).union(set(prohibit_char_lst)))
    sample = random.sample(union, min(ceil(0.1*len(union)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_ucd_prohibit():
    sample = random.sample(prohibit_char_lst, min(ceil(0.1*len(prohibit_char_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_private_use():
    sample = random.sample(private_use_lst, min(ceil(0.1*len(private_use_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_surrogate():
    return [code_to_utf8_bytes(i) for i in surrogate_lst]

def get_inappropriate_for_plain_text():
    sample = random.sample(inappropriate_for_plain_text_lst, min(ceil(0.1*len(inappropriate_for_plain_text_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_inappropriate_for_canonical_replacement():
    sample = random.sample(inappropriate_for_canonical_replacement_lst, min(ceil(0.1*len(inappropriate_for_canonical_replacement_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_change_display_properties():
    sample = random.sample(change_display_properties_lst, min(ceil(0.1*len(change_display_properties_lst)), 0x1f))
    return [code_to_utf8_bytes(i) for i in sample]

def get_tagging_character():
    return [code_to_utf8_bytes(i) for i in tagging_character_lst]

def do_nothing():
    # for test
    return []


def code_to_utf8_bytes(code: int) -> str:
    # convert a int to utf-8 encoding format bytes
    if code < 0x80:
        return bytes(chr(code), "utf-8")
    elif code < 0x800:
        return (0xc0 | code >> 6).to_bytes(1, 'big') + (0x80 | code & 0x3f).to_bytes(1, 'big')
    elif code < 0x10000:
        return (0xe0 | code >> 12).to_bytes(1, 'big') + (0x80 | code >> 6 & 0x3f).to_bytes(1, 'big') + (0x80 | code & 0x3f).to_bytes(1, 'big')
    elif code < 0x200000:
        return (0xf0 | code >> 18).to_bytes(1, 'big') + (0x80 | code >> 12 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 6 & 0x3f).to_bytes(1, 'big') + (0x80 | code & 0x3f).to_bytes(1, 'big')
    elif code < 0x4000000:
        return (0xf8 | code >> 24).to_bytes(1, 'big') + (0x80 | code >> 18 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 12 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 6 & 0x3f).to_bytes(1, 'big') + (0x80 | code & 0x3f).to_bytes(1, 'big')
    elif code <= 0x7fffffff:
        return (0xfc | code >> 30).to_bytes(1, 'big') + (0x80 | code >> 24 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 18 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 12 & 0x3f).to_bytes(1, 'big') + (0x80 | code >> 6 & 0x3f).to_bytes(1, 'big') + (0x80 | code & 0x3f).to_bytes(1, 'big')
    else:
        raise ValueError("code point out of range")

from collections import OrderedDict
get_char_func = OrderedDict([
    ('asc_space', get_asc_space),
    ('ucd_space', get_ucd_space),
    ('asc_control', get_asc_control),
    ('ucd_control', get_ucd_control),
    ('asc_char', get_asc_char),
    ('ucd_char', get_ucd_char),
    # ('nothing', do_nothing),
    # TODO: need to support more char type
    ('non_char', get_non_char),
    ('null_char', get_null_char),
    ('asc_prohibit', get_asc_prohibit),
    ('ucd_prohibit', get_ucd_prohibit),
    ('private_use', get_private_use),
    ('surrogate', get_surrogate),
    ('inappropriate_for_plain_text', get_inappropriate_for_plain_text),
    ('inappropriate_for_canonical_replacement', get_inappropriate_for_canonical_replacement),
    ('change_display_properties', get_change_display_properties),
    ('tagging_character', get_tagging_character),
])



if __name__ == "__main__":
    # test code_to_utf8 func
    # print(code_to_utf8_bytes(0x777)); print(code_to_utf8_bytes(0xdf03)); print(code_to_utf8_bytes(0x1df03))
    print(get_inappropriate_for_plain_text())