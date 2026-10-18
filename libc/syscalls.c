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
 * Thin wrappers for FreeBSD system calls that are not exported by any of
 * the sprx libraries we link with by default. Each wrapper loads the
 * syscall number into %rax and tail-calls __syscall_direct, which runs the
 * syscall instruction itself (the PS4 lets a process do so from its own
 * code) and, on failure, sets errno and returns -1, as FreeBSD's libc does.
 *
 * The PS4's kernel is FreeBSD 9's: only system calls that FreeBSD 9 has
 * are wrapped. From 532 on the numbers are Sony's own system calls, and a
 * number FreeBSD 9 does not implement raises SIGSYS.
 *
 * Arguments are passed through untouched, so no prototypes are needed here;
 * the public declarations come from the FreeBSD headers in ../include.
 **/

/**
 * Invoke the syscall whose number is already loaded into %rax, with the
 * arguments in the standard SysV registers.
 **/
__asm__(".text\n"
        ".p2align 4\n"
        ".hidden __syscall_direct\n"
        ".type __syscall_direct, @function\n"
        "__syscall_direct:\n"
        "  movq %rcx, %r10\n"
        "  syscall\n"
        "  jb 1f\n"
        "  ret\n"
        "1:\n"
        "  pushq %rax\n"            /* the error number; aligns the stack */
        "  call __error@PLT\n"
        "  popq %rcx\n"
        "  movl %ecx, (%rax)\n"
        "  movq $-1, %rax\n"
        "  ret\n"
        ".size __syscall_direct, .-__syscall_direct\n");


#define SYSCALL(name, number)                         \
  __asm__(".text\n"                                   \
          ".global " #name "\n"                       \
          ".type " #name ", @function\n"              \
          ".p2align 4\n"                              \
          #name ":\n"                                 \
          "  movl $" #number ", %eax\n"               \
          "  jmp __syscall_direct\n"                  \
          ".size " #name ", .-" #name "\n")

SYSCALL(fork                      , 2);
SYSCALL(wait4                     , 7);
SYSCALL(link                      , 9);
SYSCALL(fchdir                    , 13);
SYSCALL(mknod                     , 14);
SYSCALL(chown                     , 16);
SYSCALL(mount                     , 21);
SYSCALL(unmount                   , 22);
SYSCALL(ptrace                    , 26);
SYSCALL(ktrace                    , 45);
SYSCALL(setlogin                  , 50);
SYSCALL(acct                      , 51);
SYSCALL(reboot                    , 55);
SYSCALL(revoke                    , 56);
SYSCALL(symlink                   , 57);
SYSCALL(umask                     , 60);
SYSCALL(chroot                    , 61);
SYSCALL(sbrk                      , 69);
SYSCALL(sstk                      , 70);
SYSCALL(vadvise                   , 72);
SYSCALL(getpgrp                   , 81);
SYSCALL(setpgid                   , 82);
SYSCALL(swapon                    , 85);
SYSCALL(fchown                    , 123);
SYSCALL(mkfifo                    , 132);
SYSCALL(adjtime                   , 140);
SYSCALL(setsid                    , 147);
SYSCALL(quotactl                  , 148);
SYSCALL(nlm_syscall               , 154);
SYSCALL(nfssvc                    , 155);
SYSCALL(lgetfh                    , 160);
SYSCALL(getfh                     , 161);
SYSCALL(rtprio                    , 166);
SYSCALL(semsys                    , 169);
SYSCALL(msgsys                    , 170);
SYSCALL(shmsys                    , 171);
SYSCALL(setfib                    , 175);
SYSCALL(ntp_adjtime               , 176);
SYSCALL(setgid                    , 181);
SYSCALL(pathconf                  , 191);
SYSCALL(fpathconf                 , 192);
SYSCALL(undelete                  , 205);
SYSCALL(getpgid                   , 207);
SYSCALL(semget                    , 221);
SYSCALL(semop                     , 222);
SYSCALL(msgget                    , 225);
SYSCALL(msgsnd                    , 226);
SYSCALL(msgrcv                    , 227);
SYSCALL(shmat                     , 228);
SYSCALL(shmdt                     , 230);
SYSCALL(shmget                    , 231);
SYSCALL(clock_getcpuclockid2      , 247);
SYSCALL(ntp_gettime               , 248);
SYSCALL(minherit                  , 250);
SYSCALL(rfork                     , 251);
SYSCALL(lchown                    , 254);
SYSCALL(aio_read                  , 255);
SYSCALL(aio_write                 , 256);
SYSCALL(lio_listio                , 257);
SYSCALL(lchmod                    , 274);
SYSCALL(lutimes                   , 276);
SYSCALL(nstat                     , 278);
SYSCALL(nfstat                    , 279);
SYSCALL(nlstat                    , 280);
SYSCALL(fhopen                    , 298);
SYSCALL(fhstat                    , 299);
SYSCALL(modnext                   , 300);
SYSCALL(modstat                   , 301);
SYSCALL(modfnext                  , 302);
SYSCALL(modfind                   , 303);
SYSCALL(kldload                   , 304);
SYSCALL(kldunload                 , 305);
SYSCALL(kldfind                   , 306);
SYSCALL(kldnext                   , 307);
SYSCALL(kldstat                   , 308);
SYSCALL(kldfirstmod               , 309);
SYSCALL(setresuid                 , 311);
SYSCALL(setresgid                 , 312);
SYSCALL(aio_return                , 314);
SYSCALL(aio_suspend               , 315);
SYSCALL(aio_cancel                , 316);
SYSCALL(aio_error                 , 317);
SYSCALL(yield                     , 321);
SYSCALL(utrace                    , 335);
SYSCALL(kldsym                    , 337);
SYSCALL(jail                      , 338);
SYSCALL(nnpfs_syscall             , 339);
SYSCALL(__acl_get_file            , 347);
SYSCALL(__acl_set_file            , 348);
SYSCALL(__acl_get_fd              , 349);
SYSCALL(__acl_set_fd              , 350);
SYSCALL(__acl_delete_file         , 351);
SYSCALL(__acl_delete_fd           , 352);
SYSCALL(__acl_aclcheck_file       , 353);
SYSCALL(__acl_aclcheck_fd         , 354);
SYSCALL(extattrctl                , 355);
SYSCALL(extattr_set_file          , 356);
SYSCALL(extattr_get_file          , 357);
SYSCALL(extattr_delete_file       , 358);
SYSCALL(aio_waitcomplete          , 359);
SYSCALL(getresuid                 , 360);
SYSCALL(getresgid                 , 361);
SYSCALL(extattr_set_fd            , 371);
SYSCALL(extattr_get_fd            , 372);
SYSCALL(extattr_delete_fd         , 373);
SYSCALL(__setugid                 , 374);
SYSCALL(eaccess                   , 376);
SYSCALL(afs3_syscall              , 377);
SYSCALL(nmount                    , 378);
SYSCALL(__mac_get_proc            , 384);
SYSCALL(__mac_set_proc            , 385);
SYSCALL(__mac_get_fd              , 386);
SYSCALL(__mac_get_file            , 387);
SYSCALL(__mac_set_fd              , 388);
SYSCALL(__mac_set_file            , 389);
SYSCALL(kenv                      , 390);
SYSCALL(lchflags                  , 391);
SYSCALL(mac_syscall               , 394);
SYSCALL(getfsstat                 , 395);
SYSCALL(statfs                    , 396);
SYSCALL(fstatfs                   , 397);
SYSCALL(fhstatfs                  , 398);
SYSCALL(__mac_get_pid             , 409);
SYSCALL(__mac_get_link            , 410);
SYSCALL(__mac_set_link            , 411);
SYSCALL(extattr_set_link          , 412);
SYSCALL(extattr_get_link          , 413);
SYSCALL(extattr_delete_link       , 414);
SYSCALL(__mac_execve              , 415);
SYSCALL(swapoff                   , 424);
SYSCALL(__acl_get_link            , 425);
SYSCALL(__acl_set_link            , 426);
SYSCALL(__acl_delete_link         , 427);
SYSCALL(__acl_aclcheck_link       , 428);
SYSCALL(thr_create                , 430);
SYSCALL(thr_exit                  , 431);
SYSCALL(thr_self                  , 432);
SYSCALL(thr_kill                  , 433);
SYSCALL(jail_attach               , 436);
SYSCALL(extattr_list_fd           , 437);
SYSCALL(extattr_list_file         , 438);
SYSCALL(extattr_list_link         , 439);
SYSCALL(thr_suspend               , 442);
SYSCALL(thr_wake                  , 443);
SYSCALL(kldunloadf                , 444);
SYSCALL(audit                     , 445);
SYSCALL(auditon                   , 446);
SYSCALL(getauid                   , 447);
SYSCALL(setauid                   , 448);
SYSCALL(getaudit                  , 449);
SYSCALL(setaudit                  , 450);
SYSCALL(getaudit_addr             , 451);
SYSCALL(setaudit_addr             , 452);
SYSCALL(auditctl                  , 453);
SYSCALL(thr_new                   , 455);
SYSCALL(kmq_open                  , 457);
SYSCALL(kmq_setattr               , 458);
SYSCALL(kmq_timedreceive          , 459);
SYSCALL(kmq_timedsend             , 460);
SYSCALL(kmq_notify                , 461);
SYSCALL(kmq_unlink                , 462);
SYSCALL(abort2                    , 463);
SYSCALL(thr_set_name              , 464);
SYSCALL(aio_fsync                 , 465);
SYSCALL(sctp_peeloff              , 471);
SYSCALL(sctp_generic_sendmsg      , 472);
SYSCALL(sctp_generic_sendmsg_iov  , 473);
SYSCALL(sctp_generic_recvmsg      , 474);
SYSCALL(thr_kill2                 , 481);
SYSCALL(cpuset                    , 484);
SYSCALL(cpuset_setid              , 485);
SYSCALL(faccessat                 , 489);
SYSCALL(fchmodat                  , 490);
SYSCALL(fchownat                  , 491);
SYSCALL(fexecve                   , 492);
SYSCALL(futimesat                 , 494);
SYSCALL(linkat                    , 495);
SYSCALL(mkdirat                   , 496);
SYSCALL(mkfifoat                  , 497);
SYSCALL(mknodat                   , 498);
SYSCALL(openat                    , 499);
SYSCALL(renameat                  , 501);
SYSCALL(symlinkat                 , 502);
SYSCALL(unlinkat                  , 503);
SYSCALL(posix_openpt              , 504);
SYSCALL(gssd_syscall              , 505);
SYSCALL(jail_get                  , 506);
SYSCALL(jail_set                  , 507);
SYSCALL(jail_remove               , 508);
SYSCALL(closefrom                 , 509);
SYSCALL(msgctl                    , 511);
SYSCALL(shmctl                    , 512);
SYSCALL(lpathconf                 , 513);
SYSCALL(cap_enter                 , 516);
SYSCALL(cap_getmode               , 517);
SYSCALL(pdfork                    , 518);
SYSCALL(pdkill                    , 519);
SYSCALL(pdgetpid                  , 520);
SYSCALL(getloginclass             , 523);
SYSCALL(setloginclass             , 524);
SYSCALL(rctl_get_racct            , 525);
SYSCALL(rctl_get_rules            , 526);
SYSCALL(rctl_get_limits           , 527);
SYSCALL(rctl_add_rule             , 528);
SYSCALL(rctl_remove_rule          , 529);
SYSCALL(posix_fallocate           , 530);
SYSCALL(posix_fadvise             , 531);


/**
 * vfork() shares the address space (and stack) with the parent, and the
 * child would return through the wrapper's frame, which the parent then
 * returns through again. Fall back to a regular fork() instead.
 **/
SYSCALL(vfork                     , 2);
