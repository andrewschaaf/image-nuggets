
import sys, os, tempfile, subprocess
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



def pngOfUrl(url, delay=1.0):
    with TempDir() as td:
        
        destPath = '%s/foo-full.png' % td.path
        
        p = subprocess.Popen(['/usr/bin/env', 'python', '%s/lib/webkit2png.py' % REPO,
                                '--fullsize', # full only
                                '--filename=' + 'foo',
                                '--delay=' + str(delay),
                                url], cwd=td.path,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)
        out, err = p.communicate()
        if p.returncode != 0:
            raise Exception('webkit2png subprocess error\n\n' + repr(err))
        
        with open(destPath, 'rb') as f:
            return f.read()


def pngOfHtml(html, **kwargs):
    with TempDir() as td:
        path = os.path.abspath('%s/foo.html' % td.path)
        url = 'file://%s' % path
        with open(path, 'wb') as f:
            f.write(html)
        return pngOfUrl(url, **kwargs)


def convertImageData(data, fromExt, toExt):
    with TempDir() as td:
        tmpdir = '/Users/a/Desktop'
        src = '%s/foo.%s' % (tmpdir, fromExt)
        dest = '%s/foo.%s' % (tmpdir, toExt)
        with open(src, 'wb') as f:
            f.write(data)
        #with open(src, 'rb') as f:
        #    raise Exception(repr(f.read()))
        
        p = subprocess.Popen(['convert', src, dest],
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)
        out, err = p.communicate()
        
        if p.returncode != 0:
            raise Exception(err)
        with open(dest, 'rb') as f:
            return f.read()


class TempDir:
    
    def __init__(self):
        self.path = tempfile.mkdtemp()
    
    def __enter__(self):
        return self
    
    def __exit__(self, *args):
        subprocess.check_call(['rm', '-rf', self.path])


