from .char_table import *

oid_dict = {
    'utf8': 12,
    'printable': 19,
    'teletex': 20,
    'universal': 28,
    'bmp': 30,
    # the above is allowed in rfc 5280 subject/issuer field
    'ia5': 22,
    'general': 27,
    'graphic': 25,
    'numeric': 18,
    'videotex': 21,
    'visible': 26,
}

string_charset = {
    'utf8': utf8_lst,
    'printable': printable_lst,
    'teletex': teletex_lst,
    'universal': universal_lst,
    'bmp': bmp_lst,
    # the above is allowed in rfc 5280 subject/issuer field
    'ia5': ia5_lst,
    'general': general_lst,
    'graphic': graphic_lst,
    'numeric': numeric_lst,
    'videotex': videotex_lst,
    'visible': visible_lst,
}
