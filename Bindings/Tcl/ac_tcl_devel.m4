###############################################################################
# libbrlapi - A library providing access to braille terminals for applications.
#
# Copyright (C) 2006-2007 by Dave Mielke <dave@mielke.cc>
#
# libbrlapi comes with ABSOLUTELY NO WARRANTY.
#
# This is free software, placed under the terms of the
# GNU Lesser General Public License, as published by the Free Software
# Foundation; either version 2.1 of the License,
# or (at your option) any later version.
# Please see the file COPYING-API for details.
#
# Web Page: http://mielke.cc/brltty/
#
# This software is maintained by Dave Mielke <dave@mielke.cc>.
###############################################################################

AC_DEFUN([AC_TCL_DEVEL], [dnl
TCL=false
TCL_INCLUDE_FLAGS=""
TCL_LIB_FLAGS=""

BRLTTY_ARG_WITH(
   [tcl-config], [PATH],
   [the path to the Tcl configuration script (tclConfig.sh) or to the directory containing it],
   [tcl_config_script], ["no"]
)

tcl_config_name="tclConfig.sh"
if test "${tcl_config_script}" = "no"
then
   AC_CHECK_HEADER([tcl.h], [dnl
      TCL=true
      TCL_LIB_FLAGS="-ltcl"
   ])
else
   test "${tcl_config_script}" != "yes" || tcl_config_script=""

   if test -z "${tcl_config_script}"
   then
      for directory in "/usr/lib" "/usr/local/lib"
      do
         script="${directory}/${tcl_config_name}"
         test ! -f "${script}" || {
            tcl_config_script="${script}"
            AC_MSG_NOTICE([Tcl configuration script is ${tcl_config_script}])
            break
         }
      done

      test -n "${tcl_config_script}" || {
         AC_MSG_WARN([Tcl configuration script not found: ${tcl_config_name}])
      }
   else
      test ! -d "${tcl_config_script}" || tcl_config_script="${tcl_config_script}/${tcl_config_name}"

      test -f "${tcl_config_script}" || {
         AC_MSG_WARN([Tcl configuration script not found: ${tcl_config_script}])
         tcl_config_script=""
      }
   fi

   test -z "${tcl_config_script}" || {
      if test ! -r "${tcl_config_script}"
      then
         AC_MSG_WARN([Tcl configuration script not readable: ${tcl_config_script}])
      elif . "${tcl_config_script}"
      then
         TCL=true
         TCL_INCLUDE_FLAGS="${TCL_INCLUDE_SPEC}"
         TCL_LIB_FLAGS="${TCL_LIB_SPEC}"
      fi
   }
fi

"${TCL}" && {
   AC_PATH_PROGS([TCLSH], [tclsh${TCL_VERSION} tclsh], [TCLSH_NOT_FOUND_BY_CONFIGURE])
}

AC_SUBST([TCL])
AC_SUBST([TCL_INCLUDE_FLAGS])
AC_SUBST([TCL_LIB_FLAGS])
])
