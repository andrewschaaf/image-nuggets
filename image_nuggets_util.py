
import sys, os
from cStringIO import StringIO


def parentOf(path, n=1):
    return '/'.join(path.rstrip('/').split('/')[:-n])


REPO = parentOf(os.path.abspath(__file__))


def fatalError(msg):
    sys.stderr.write('{stars} OH NOES {stars}\n{msg}\n'.format(
                            stars='*' * 25,
                            msg=msg))
    sys.exit(1)


def readBlobs(f):
    
    if isinstance(f, str):
        f = StringIO(f)
    
    while True:
        numline = f.readline()
        if not numline.strip():
            return
        vlen = int(numline)
        v = f.read(vlen)
        assert len(v) == vlen
        yield v


# TODO: remove external deps
import os
from a.temp import TempDir
from a.shell import check_communicate


def pngOfUrl(url, delay=1.0):
    with TempDir() as td:
        
        destPath = '%s/foo-full.png' % td.path
        
        check_communicate(['/usr/bin/env', 'python', '%s/lib/webkit2png.py' % REPO,
                                '--fullsize', # full only
                                '--filename=' + 'foo',
                                '--delay=' + str(delay),
                                url], cwd=td.path)
        
        with open(destPath, 'rb') as f:
            return f.read()


def pngOfHtml(html, **kwargs):
    with TempDir() as td:
        path = os.path.abspath('%s/foo.html' % td.path)
        url = 'file://%s' % path
        with open(path, 'wb') as f:
            f.write(html)
        return pngOfUrl(url, **kwargs)


# TODO: use stdio, remove external deps
from a.shell import communicateWithReturncode
from a.temp import TempDir
def convertImageData(data, fromExt, toExt):
    with TempDir() as td:
        tmpdir = '/Users/a/Desktop'
        src = '%s/foo.%s' % (tmpdir, fromExt)
        dest = '%s/foo.%s' % (tmpdir, toExt)
        with open(src, 'wb') as f:
            f.write(data)
        #with open(src, 'rb') as f:
        #    raise Exception(repr(f.read()))
        out, err, returncode = communicateWithReturncode(['convert', src, dest])
        if returncode != 0:
            raise Exception(err)
        with open(dest, 'rb') as f:
            return f.read()
