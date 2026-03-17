# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-src"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-build"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/tmp"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/src/glew-populate-stamp"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/src"
  "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/src/glew-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/src/glew-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/alfa/programmer_socks/git/PA5_BLACKJACK/build/_deps/glew-subbuild/glew-populate-prefix/src/glew-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
