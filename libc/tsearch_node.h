/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

/**
 * The node of FreeBSD's balanced tsearch() tree. The SDK's <search.h> is
 * FreeBSD 9's, whose tsearch() family takes void pointers and whose
 * private node has no balance, so its prototypes are kept out of the way
 * here; the functions are the same to a caller.
 **/

#ifndef TSEARCH_NODE_H
#define TSEARCH_NODE_H

#define tdelete __search_h_tdelete
#define tfind   __search_h_tfind
#define tsearch __search_h_tsearch
#define twalk   __search_h_twalk
#include <search.h>
#undef tdelete
#undef tfind
#undef tsearch
#undef twalk

typedef struct __posix_tnode {
  void                 *key;
  struct __posix_tnode *llink, *rlink;
  signed char           balance;
} posix_tnode;

#endif /* TSEARCH_NODE_H */
