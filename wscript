# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='0.1'
APPNAME='NdnKeyManagementTool'

from waflib import Build, Logs, Utils, Task, TaskGen, Configure

def options(opt):
    opt.add_option('--debug',action='store_true',default=False,dest='debug',help='''debugging mode''')
    # opt.add_option('--test', action='store_true',default=False,dest='_test',help='''build unit tests''')

    opt.load('compiler_c compiler_cxx boost ccnx qt4')

def configure(conf):
    conf.load("compiler_c compiler_cxx")

    if conf.options.debug:
        conf.define ('_DEBUG', 1)
        conf.add_supported_cxxflags (cxxflags = ['-O0',
                                                 '-Wall',
                                                 '-Wno-unused-variable',
                                                 '-g3',
                                                 '-Wno-unused-private-field', # only clang supports
                                                 '-fcolor-diagnostics',       # only clang supports
                                                 '-Qunused-arguments'         # only clang supports
                                                 ])
    else:
        conf.add_supported_cxxflags (cxxflags = ['-O3', '-g'])

    conf.define ("KEY_MANAGEMENT_VERSION", VERSION)

    conf.check_cfg(package='libccnx-cpp', args=['--cflags', '--libs'], uselib_store='NDNCXX', mandatory=True)

    conf.check_cfg(package='sqlite3', args=['--cflags', '--libs'], uselib_store="SQLITE3", mandatory=True)


    if not conf.check_cfg(package='openssl', args=['--cflags', '--libs'], uselib_store='SSL', mandatory=False):
        libcrypto = conf.check_cc(lib='crypto',
                                  header_name='openssl/crypto.h',
                                  define_name='HAVE_SSL',
                                  uselib_store='SSL')
    else:
        conf.define ("HAVE_SSL", 1)
    if not conf.get_define ("HAVE_SSL"):
        conf.fatal ("Cannot find SSL libraries")

    conf.load ('ccnx')
    conf.load('qt4')
    conf.load('boost')
    conf.load('openssl')
    conf.load('sqlite3')
    conf.check_boost(lib='system test thread')

    # boost_version = conf.env.BOOST_VERSION.split('_')
    # if int(boost_version[0]) < 1 or int(boost_version[1]) < 46:
    #     Logs.error ("Minumum required boost version is 1.46")
    #     return

    conf.check_ccnx (path=conf.options.ccnx_dir)
    conf.define ('CCNX_PATH', conf.env.CCNX_ROOT)

    # if conf.options._test:
    #     conf.define ('_TESTS', 1)
    #     conf.env.TEST = 1

    conf.write_config_header('src/config.h')

def build (bld):
    # # Unit tests
    # if bld.env['TEST']:
    #   unittests = bld.program (
    #       target="unit-tests",
    #       features = "qt4 cxx cxxprogram",
    #       defines = "WAF",
    #       source = bld.path.ant_glob(['test/*.cc']),
    #       use = 'BOOST_TEST BOOST_FILESYSTEM BOOST_DATE_TIME LOG4CXX SQLITE3 QTCORE QTGUI ccnx database fs_watcher chronoshare',
    #       includes = "ccnx scheduler src executor gui fs-watcher",
    #       install_prefix = None,
    #       )

    qt = bld (
        target = "KeyTool",
        features = "qt4 cxx cxxprogram",
        defines = "WAF",
        source = bld.path.ant_glob(['src/**/*.cpp', 
                                    'src/**/*.ui', 
                                    'src/**/*.qrc']),
        includes = ". src",
        use = "QTCORE QTGUI  SQLITE3 NDNCXX BOOST BOOST_SYSTEM",
        )

    if Utils.unversioned_sys_platform () == "darwin":
        app_plist = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd">
<plist version="0.9">
<dict>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleGetInfoString</key>
    <string>Created by Waf</string>
    <key>CFBundleIdentifier</key>
    <string>edu.ucla.cs.irl.KeyTool</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>NOTE</key>
    <string>THIS IS A GENERATED FILE, DO NOT MODIFY</string>
    <key>CFBundleExecutable</key>
    <string>%s</string>
</dict>
</plist>'''
        qt.mac_app = "KeyTool.app"
        qt.mac_plist = app_plist % "KeyTool"
        # qt.mac_resources = 'chronoshare.icns'
        # qt.use += " OSX_FOUNDATION OSX_COREWLAN adhoc"

    # <key>LSUIElement</key>
    # <string>1</string>

    # <key>CFBundleIconFile</key>
    # <string>chronoshare.icns</string>

    # <key>SUPublicDSAKeyFile</key>
    # <string>dsa_pub.pem</string>
    # <key>CFBundleIconFile</key>
    # <string>chronoshare.icns</string>


@Configure.conf
def add_supported_cxxflags(self, cxxflags):
    """
    Check which cxxflags are supported by compiler and add them to env.CXXFLAGS variable
    """
    self.start_msg('Checking allowed flags for c++ compiler')

    supportedFlags = []
    for flag in cxxflags:
        if self.check_cxx (cxxflags=[flag], mandatory=False):
            supportedFlags += [flag]

    self.end_msg (' '.join (supportedFlags))
    self.env.CXXFLAGS += supportedFlags
