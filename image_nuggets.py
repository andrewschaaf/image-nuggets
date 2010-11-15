#!/usr/bin/env python

import os, sys, optparse, subprocess
from image_nuggets_util import pngOfHtml, convertImageData, fatalError, parentOf, readBlobs


REPO = parentOf(os.path.abspath(__file__))


def main():
    
    # Parse options
    parser = optparse.OptionParser()
    parser.add_option('-f', '--from', dest='fromFormat', default=None)
    parser.add_option('-t', '--to', dest='toFormat', default=None)
    parser.add_option('-p', '--dest-prefix', dest='destPrefix', default=None)
    parser.add_option('-s', '--dest-suffixes', dest='destSuffixes', default=None)
    parser.add_option('-O', '--optipng-level', dest='optipngLevel', default=None)
    options, args = parser.parse_args()
    
    # Validate options
    assert len(args) == 0
    assert options.fromFormat
    assert options.toFormat
    assert options.destPrefix
    assert options.destSuffixes
    destSuffixes = options.destSuffixes.split(',')
    
    # Get the subimages
    data = sys.stdin.read()
    ppm = ppmFromInput(data, options.fromFormat)
    ppms = subppmsOf(ppm)
    if len(destSuffixes) != len(ppms):
        fatalError('Only %d dest-suffixes for %d subimages!' % (len(destSuffixes), len(ppms)))
    
    # Write the subimages
    for ppm, destSuffix in zip(ppms, destSuffixes):
        destPath = '%s%s' % (options.destPrefix, destSuffix)
        folder = parentOf(destPath)
        if not os.path.isdir(folder):
            subprocess.check_call(['mkdir', '-p', folder])
        writePpmTo(ppm, destPath, options.optipngLevel)


def ppmFromInput(data, format):
    
    if format == 'ppm':
        ppm = data
    
    elif format == 'html':
        png = pngOfHtml(data)
        ppm = ppmFromInput(png, 'png')
    
    else:
        ppm = convertImageData(data, format, 'ppm')
    
    assert ppm.startswith('P6')
    return ppm


def subppmsOf(ppm):
    
    binPath = '%s/bin/image-nuggets-c' % REPO
    if not os.path.isfile(binPath):
        fatalError('You need to compile image-nuggets. Run REPO/make.sh')
    
    p = subprocess.Popen([binPath],
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    out, err = p.communicate(input=ppm)
    
    if p.returncode != 0:
        fatalError('Error while running C subprocess:\n\n%s' % err)
    
    return list(readBlobs(out))


def writePpmTo(data, destPath, optipngLevel):
    
    p = subprocess.Popen(
                        ['/usr/bin/env', 'convert', '-', destPath],
                        stdin=subprocess.PIPE,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)
    p.communicate(input=data)
    
    if optipngLevel is not None:
        optipngLevel = int(optipngLevel)
        p = subprocess.Popen(['/usr/bin/env',
                                        'optipng',
                                        '-o%d' % optipngLevel,
                                        destPath],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)
        out, err = p.communicate()
        if p.returncode != 0:
            raise Exception('Error while running optipng!\n\n' + repr(err))


if __name__ == '__main__':
    main()
