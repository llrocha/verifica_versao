#!C:\python\Python34\python.exe -u
#!/usr/bin/env python

import os
import cgi
import glob
import stat
import hashlib
import datetime

import cgitb
cgitb.enable()  # for troubleshooting

def stat_file(fname):
    st = os.stat(fname)
    mtime = datetime.datetime.fromtimestamp(st.st_mtime)
    ctime = datetime.datetime.fromtimestamp(st.st_ctime)
    filetype = 'f'
    if(stat.S_ISBLK(st.st_mode)):
        filetype = 'b'
    elif(stat.S_ISCHR(st.st_mode)):
        filetype = 'c'
    elif(os.path.isdir(fname)):
        filetype = 'd'
    elif(os.path.islink(fname)):
        filetype = 'l'
    elif(os.path.ismount(fname)):
        filetype = 'd'

    r = '{0}.{1:04}.{2:04}.{3:04}.{4}.{5}'.format(
        'f', 
        st.st_uid,
        st.st_gid,
        int(str(oct(0o7777 & st.st_mode)[2:])),
        mtime.strftime('%Y-%m-%d.%H:%M:%S'),
        ctime.strftime('%Y-%m-%d.%H:%M:%S')
    )

    return r

def versao_md5(fname):
    count = 0
    versao = '0'
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for byte in iter(lambda: f.read(1), b""):
            hash_md5.update(byte)
            if(byte == b'{'):
                count += 1
            elif(byte == b'@' and count > 0):
                count += 1
            elif(count == 4):
                count += 1
            elif(count == 5):
                versao = byte.decode()
                count = 0
            else:
                count = 0

    return [versao, hash_md5.hexdigest(), stat_file(fname), fname]

print("Content-type: text/plain")
print()

form = cgi.FieldStorage()
path = form.getvalue("d", ".")
mask = form.getvalue("f", "*")

for file in glob.glob('{0}/{1}'.format(path, mask)):
    print('{0}\t{1}\t{2}\t{3}'.format(*versao_md5(file)))