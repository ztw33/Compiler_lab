import os

def gen(path):
    files = os.listdir(path)

    for f in files:
        if f.endswith('.cmm'):
            os.system('./Code/parser ' + path + f + ' ' + path + f.replace('.cmm', '.s'))

gen('Test/')
gen('Tests1/')
gen('Tests2/')
