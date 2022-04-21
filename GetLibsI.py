#!/usr/bin/env python
# PYTHON_ARGCOMPLETE_OK
# Common tools:
#	pip/pip3 : install packages
#	pylint/pychecker : verify syntax
#	unittest package for xUnit.
""" Build C libraries for development.
    This version is inlined to be inedependent of SysInstall.py
"""

# Imports
from __future__ import print_function
import argparse
import functools
import glob
import itertools
import multiprocessing
import os
import pathlib
import re
import shlex
import shutil
import subprocess
import tarfile
import zipfile
try:
    from argcomplete import autocomplete
except ImportError:
    def autocomplete(dummy):
        """ Dummy func. """
        pass


URL_ARGTABLE = 'http://prdownloads.sourceforge.net/argtable/argtable2-13.tar.gz'
URL_BOOST = 'https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2'
URL_GTEST = 'https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz'
URL_GNUMP = 'https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2'

TMP_DIR = '/tmp/SysInstall'
BUILDS = {
    'cppunit': {
        'name' : 'cppunit',
        'check': 'lib/libcppunit.a',
        'url'  : 'git://anongit.freedesktop.org/git/libreoffice/cppunit/',
        'cmds' : [
            './autogen.sh',
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
    'gtest': {
        'name' : 'gtest',
        'check': 'lib/libgtest.a',
        'url'  : URL_GTEST,
        'cmds' : [
            'cmake .',
            'make -C googlemock',
        ],
        'globs': [
            ('googletest/include/gtest/*', 'include/gtest/'),
            ('googletest/include/gtest/internal/*', 'include/gtest/internal/'),
            ('googletest/include/gtest/internal/custom/*', 'include/gtest/internal/custom/'),
            ('googlemock/include/gmock/*', 'include/gmock/'),
            ('googlemock/include/gmock/internal/*', 'include/gmock/internal/'),
            ('lib/*.a', 'lib/'),
        ],
    },
    'boost': {
        'name' : 'boost',
        'check': 'lib/libboost_thread.a',
        'url'  : URL_BOOST,
        'cmds' : [
            './bootstrap.sh --prefix=TARGET',
            './b2 install',
        ],
        'globs': [
            ('libs/date_time/data/*', 'share/boost/date_time/'),
        ],
    },
    'gnump': {
        'name': 'gnump',
        'check': 'lib/libgmp.a',
        'url': URL_GNUMP,
        'cmds': [
            './configure --prefix=TARGET --disable-shared --with-pic',
            'make -jJOBS',
            'make install',
        ],
    },
    'jsoncpp': {
        'name' : 'jsoncpp',
        'check': 'lib/libjsoncpp.so',
        'url'  : 'https://github.com/open-source-parsers/jsoncpp',
        'cmds' : [
            'cmake -DCMAKE_INSTALL_PREFIX=TARGET -DCMAKE_BUILD_TYPE=debug '
                    '-DJSONCPP_LIB_BUILD_SHARED=ON .',
            'make install',
        ],
    },
    'jsonrpc': {
        'name' : 'jsonrpc',
        'check': 'lib/libjsonrpc.so',
        'url'  : 'https://github.com/cinemast/libjson-rpc-cpp',
        'cmds' : [
            'cmake -DCMAKE_INSTALL_PREFIX=TARGET .',
            'make -jJOBS install',
        ],
    },
    'SDL': {
        'name' : 'SDL',
        'check': 'lib/libSDL.a',
        'url'  : 'http://hg.libsdl.org/SDL',
        'cmds' : [
            'hg update SDL-1.2',
            './autogen.sh',
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
    'SDL2': {
        'name' : 'SDL2',
        'check': 'lib/libSDL2.a',
        'url'  : 'http://hg.libsdl.org/SDL',
        'cmds' : [
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
    'argtable': {
        'name' : 'argtable',
        'check': 'lib/libargtable2.a',
        'url'  : URL_ARGTABLE,
        'cmds' : [
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
    'cunit': {
        'name' : 'cunit',
        'check': 'lib/libcunit.a',
        'url'  : 'svn://svn.code.sf.net/p/cunit/code/trunk',
        'cmds' : [
            'sh ./bootstrap TARGET',
            'make -jJOBS install',
        ],
    },
    'libuv': {
        'name': 'libuv',
        'check': 'lib/libuv.so',
        'url': 'https://github.com/libuv/libuv',
        'cmds': [
            'sh autogen.sh',
            './configure --prefix=TARGET',
            'make install',
        ],
    },
    'jansson': {
        'name' : 'jansson',
        'check': 'lib/libjansson.so',
        'url'  : 'https://github.com/akheron/jansson',
        'cmds' : [
            'autoreconf -i',
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
    'libxml': {
        'name' : 'libxml',
        'check': 'lib/libxml2.so',
        'url'  : 'ftp://xmlsoft.org/libxml2/libxml2-git-snapshot.tar.gz',
        'cmds' : [
            './configure --prefix=TARGET',
            'make -jJOBS install',
        ],
    },
}

# Classes


class WorkerInterrupted(Exception):
    pass


class ArchiveNotSupported(Exception):
    """ Archive can't be processed. """
    pass


class PDir(object):
    """ Pushd analog for personal use. """
    dirs = []

    @staticmethod
    def push(new_dir):
        """ Push curdir to stack and change to new_dir. """
        curdir = os.path.abspath(os.curdir)
        PDir.dirs.append(curdir)
        os.chdir(new_dir)
        print("%s >>> %s" % (curdir, new_dir))

    @staticmethod
    def pop():
        """ Pop the dirstack and return to it. """
        old_dir = PDir.dirs.pop()
        os.chdir(old_dir)
        print(">>> %s" % old_dir)


def find_archive(url):
    """ Given a url, returns archive name found inside.
    If extension not supported throws excetion. """
    arc_ext = None
    for ext in ['.tgz', '.tbz2', '.tar.bz2', '.tar.gz', 'tar.xz',
                '.xz', '.rar', '.zip', '.7z']:
        right = url.rfind(ext)
        if right != -1:
            right += len(ext)
            left = url.rfind(os.sep, 0, right) + 1
            arc_ext = ext
            break

    if arc_ext is None:
        raise ArchiveNotSupported

    return url[left:right]


def extract_archive(archive):
    """ Given an archive, extract it. Prefer python libs if supported. """
    if tarfile.is_tarfile(archive):
        tarf = tarfile.open(archive)
        tarf.extractall()
    elif zipfile.is_zipfile(archive):
        zipf = zipfile.ZipFile(archive)
        zipf.extractall()
    else:
        # Fall back to shell tools to extract
        unarc_src = 'https://raw.githubusercontent.com/starcraftman/' \
                    '.my_scripts/master/unarchive'
        cmds = ['curl -o una ' + unarc_src, 'bash ./una ' + archive]
        for cmd in cmds:
            proc = subprocess.Popen(shlex.split(cmd))
            proc.wait()
            if proc.returncode != 0:
                raise OSError('Some shell util like curl unavailable.')
        os.remove('una')


def get_archive(url, target):
    """ Fetch an archive from a site. Works on regular ftp & sourceforge.
    Wish sourceforge wasn't a pain...

    url: location to get archive
    target: where to extract to
    """
    arc_name = find_archive(url)

    tmp_file = TMP_DIR + os.path.sep + arc_name
    if not os.path.exists(TMP_DIR):
        os.makedirs(TMP_DIR)

    if not os.path.exists(target):
        os.makedirs(target)
        os.rmdir(target)

    try:
        extracted = None
        # Using wget because of sourceforge corner case
        cmd = 'wget -O %s %s' % (tmp_file, url)
        subprocess.call(shlex.split(cmd))

        here = pathlib.Path(os.path.curdir)
        before = set(here.glob('*'))
        extract_archive(tmp_file)
        extracted = list(set(here.glob('*')) - before)[0]

        os.rename(extracted, target)
    except IndexError:
        print("Error during extraction and rename process, check: {}", here)
    finally:
        if os.path.exists(tmp_file):
            os.remove(tmp_file)
        if extracted and os.path.exists(extracted):
            shutil.rmtree(extracted)


def get_code(url, target):
    """ Wrapper function to clone repos, only executes if target doesn't exist
    url: The origin to clone
    target: Where to clone to
    """
    cmd = ' %s %s' % (url, target)
    # Git urls always end in .git
    if url.find('git') != -1:
        cmd = 'git clone --depth 1' + cmd
    # svn always at front of proto
    elif url.find('svn') != -1:
        cmd = 'svn checkout' + cmd
    else:
        cmd = 'hg clone' + cmd

    if not os.path.exists(target):
        subprocess.call(shlex.split(cmd))


def build_src(build, target=None):
    """ Build a project downloeaded from url. build is a json object.
        The format is described below.
        Cmds are executed in srcdir, then if globs non-empty copy files as
        described in glob/target pairs.
        Required names prefixed with R.
        {
          R 'name': 'ack',
          R 'check': 'path/to/check',
          R 'url' : 'https://github.com/petdance/ack2.git',
            'tdir': /path/to/install/to,
            'cmds': [
                'perl Makefile.PL',
                'make ack-standalone',
                'make manifypods'
            ],
            'globs': [
                ('ack-standalone', 'bin/ack'),
                ('blib/man1/*.1*', 'share/man/man1')
            ]
        }
    """
    tdir = os.path.abspath(build.get('tdir', target))
    srcdir = '%s/src/%s' % (tdir, build['name'])

    # Guard if command exists
    if os.path.exists(tdir + os.sep + build['check']):
        return

    try:
        get_archive(build['url'], srcdir)
    except ArchiveNotSupported:
        get_code(build['url'], srcdir)

    try:
        # Code should be at srcdir by here.
        PDir.push(srcdir)
        for cmd in build.get('cmds', []):
            cmd = cmd.replace('TARGET', tdir)
            cmd = cmd.replace('JOBS', '%d' % multiprocessing.cpu_count())
            subprocess.call(shlex.split(cmd))
        PDir.pop()

        # Manual copies sometimes required to finish install
        for pattern, target in build.get('globs', []):
            dest = tdir + os.sep + target
            if dest.endswith('/') and not os.path.exists(dest):
                os.makedirs(dest)

            for sfile in glob.glob(srcdir + os.sep + pattern):
                if os.path.isfile(sfile):
                    shutil.copy(sfile, dest)
    finally:
        shutil.rmtree(srcdir)


def build_wrap(args):
    """ Wrapper for build_src in process pool.
        Pool doesn't handle interrupt well, throw a different one. """
    try:
        build_src(*args)
    except KeyboardInterrupt:
        raise WorkerInterrupted


def build_pool(builds, target):
    """ Take a series of build objects and use a pool of workers
        to build them and install to target.
        NB: Blocks until all workers finished.
    """
    pool_args = zip(builds, itertools.repeat(target))
    pool = multiprocessing.Pool()
    try:
        pool.map(build_wrap, pool_args, 1)
        pool.close()
    except IOError as exc:
        print('Failed to install: {}'.format(exc))
        pool.terminate()
    except (KeyboardInterrupt, Exception) as exc:
        pool.terminate()
        print("Stopping early.")
        raise
    finally:
        pool.join()


def main():
    """ Main function. """
    mesg = """This script installs locally c libs.

    choice      desciption

    C++ Libraries
    ------------------------------------------------------
    cppunit     Install the cppunit library.
    gtest       Install the gtest library.
    boost       Install latest boost dev library.
    gnump       Install the GNU multiprecision library.
    jsonrpc     Install jsonrpc-cpp library. Beta lib & build not working atm.
    jsoncpp     Install json parsing library.
    SDL         Install the SDL1.xx game library.
    SDL2        Install the SDL2.xx game library.

    C Libraries
    ------------------------------------------------------
    argtable    Install args parsing library..
    cunit       Install the cunit test library.
    jansson     Install a json parsing library.
    libuv       Install an async IO library.
    libxml      Install a xml parsing library.
    """
    parser = argparse.ArgumentParser(
        description=mesg,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        '-l', '--ldir', nargs='?', default='libs',
        help='library directory to install to'
    )
    parser.add_argument(
        'libs', nargs='+', help='libs selected for install',
        choices=sorted(BUILDS.keys(), key=str.lower)
    )

    autocomplete(parser)
    args = parser.parse_args()  # Default parses argv[1:]
    ldir = os.path.abspath(args.ldir)

    builds = []
    actions = {}
    for key in BUILDS.keys():
        actions[key] = functools.partial(builds.append, BUILDS[key])

    try:
        # Need this for jam to build mpi & graph_parallel.
        config = os.path.expanduser('~') + os.sep + 'user-config.jam'
        with open(config, 'w') as f_conf:
            f_conf.write('using mpi ;')

        for lib in args.libs:
            actions[lib]()

        # Multiprocess to overlap builds
        build_pool(builds, ldir)
    finally:
        try:
            os.remove(config)
            os.removedirs(ldir + os.path.sep + 'src')
            os.removedirs(ldir)
        except OSError:
            pass


if __name__ == '__main__':
    main()
