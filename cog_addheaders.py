# Copyright Hugh Perkins 2014 hughperkins at gmail

# This Source Code Form is subject to the terms of the Mozilla Public License, 
# v. 2.0. If a copy of the MPL was not distributed with this file, You can 
# obtain one at http://mozilla.org/MPL/2.0/.
# 
# simply import this module in your header file like:
#
#    // [[[cog
#    // import cog_addheaders
#    // cog_addheaders.add()
#    // ]]]
#    // [[[end]]]
#
# ... and run cog on the header file, to generate the header declarations

import cog

def add():
#    debug = open('debug.txt', 'a' )
#    debug.write( 'foo\n')
#    debug.write( 'infile [' + cog.inFile + ']\n' )

    infile = cog.inFile
    cppfile = infile.replace('.h','.cpp')
    splitinfile = infile.split('/')
    infilename = splitinfile[ len(splitinfile) - 1 ]
    classname = infilename.replace('.h','')
    cog.outl( '// classname: ' + classname )
    cog.outl( '// cppfile: ' + infilename.replace('.h','.cpp' ) )
    f = open( cppfile, 'r')
    line = f.readline()
    cog.outl('')
    is_static = False
    is_virtual = False
    while( line != '' ):
       # cog.outl(line)
       if( line.find( classname + '::' ) >= 0 and line.find("(") >= 0 and line.strip().find("//") != 0 ):
           fnheader = line.replace( classname + '::', '' )
           fnheader = fnheader.replace( '{', '' )
           fnheader = fnheader.replace( ':', '' )
           if fnheader.find("[virtual]") >= 0:
               is_virtual = True
           fnheader = fnheader.replace( '// [virtual]', '' ).strip()
           fnheader = fnheader.replace( '//[virtual]', '' ).strip()
           if fnheader.find("[static]") >= 0:
              is_static = True
           fnheader = fnheader.replace( '// [static]', '' ).strip()
           fnheader = fnheader.replace( '//[static]', '' ).strip()
           fnheader = fnheader.strip().replace( ')', ');' )
           fnheader = fnheader.strip().replace( ';const', 'const;' )
           fnheader = fnheader.strip().replace( '; const', ' const;' )
           if is_static:
               cog.out( "static " )
           if is_virtual:
               cog.out( "virtual " )
           cog.outl( fnheader );
       elif( line.strip().replace(' ', '') == '//[static]' ):
           is_static = True
       elif( line.strip().replace(' ', '') == '//[virtual]' ):
           is_virtual = True
       elif( line.find('}') >= 0 ):
           is_virtual = False
           is_static = False
       line = f.readline()
    f.close()
    cog.outl('')

#    debug.close()

