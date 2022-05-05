import pandas as pds

tables = pds.read_html("https://wiki.osdev.org/PS/2_Keyboard")
tab = tables[9]

codes = []
keys = []

codes.extend(tab["Scan code"].astype('string').to_list())
codes.extend(tab["Scan code.1"].astype('string').to_list())
codes.extend(tab["Scan code.2"].astype('string').to_list())
codes.extend(tab["Scan code.3"].astype('string').to_list())

keys.extend(tab["Key"].astype('string').to_list())
keys.extend(tab["Key.1"].astype('string').to_list())
keys.extend(tab["Key.2"].astype('string').to_list())
keys.extend(tab["Key.3"].astype('string').to_list())

def decodeCode(code):
    parts = code.split(', ')
    num = 0x0000
    for p in parts:
        num <<= 8
        hexNum = p[2:]
        hexByte = int(hexNum, base=16)
        num |= hexByte
    return num

def decodeKey(keyName):
    parts = keyName.split(' ')
    nameParts = parts[:-1]
    name = ''
    for p in nameParts:
        name += p + "_"
    name = 'KEY_' + name[:-1].upper()
    action = parts[len(parts)-1]
    return name, action

for i in range(0, len(codes)):
    code = codes[i]
    key = keys[i]
    
    if pds.isna(key):
        continue

    name, type = decodeKey(key)
    if type == 'released':
        continue

    codeNum = decodeCode(code)
    print(name, '=', hex(codeNum) + ',')
