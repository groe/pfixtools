/******************************************************************************/
/*          pfixtools: a collection of postfix related tools                  */
/*          ~~~~~~~~~                                                         */
/*  ________________________________________________________________________  */
/*                                                                            */
/*  Redistribution and use in source and binary forms, with or without        */
/*  modification, are permitted provided that the following conditions        */
/*  are met:                                                                  */
/*                                                                            */
/*  1. Redistributions of source code must retain the above copyright         */
/*     notice, this list of conditions and the following disclaimer.          */
/*  2. Redistributions in binary form must reproduce the above copyright      */
/*     notice, this list of conditions and the following disclaimer in the    */
/*     documentation and/or other materials provided with the distribution.   */
/*  3. The names of its contributors may not be used to endorse or promote    */
/*     products derived from this software without specific prior written     */
/*     permission.                                                            */
/*                                                                            */
/*  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE     */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR        */
/*  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS    */
/*  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR    */
/*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF      */
/*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS  */
/*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN   */
/*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)   */
/*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF    */
/*  THE POSSIBILITY OF SUCH DAMAGE.                                           */
/******************************************************************************/

/*
 * Copyright © 2007 Pierre Habouzit
 */

#include "epoll.h"

static int epollfd = -1;

static int epoll_initialize(void)
{
    epollfd = epoll_create(128);
    return epollfd < 0 ? -1 : 0;
}

static void epoll_shutdown(void)
{
    close(epollfd);
}

module_init(epoll_initialize);
module_exit(epoll_shutdown);

void epoll_register(int fd, uint32_t events, void *ptr)
{
    struct epoll_event evt = { .events = events, .data.ptr = ptr };
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &evt)) {
        UNIXERR("epoll_ctl");
        abort();
    }
}

void epoll_modify(int fd, uint32_t events, void *ptr)
{
    struct epoll_event evt = { .events = events, .data.ptr = ptr };
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &evt)) {
        UNIXERR("epoll_ctl");
        abort();
    }
}

int epoll_select(struct epoll_event *events, int maxevents, int timeout)
{
    return epoll_wait(epollfd, events, maxevents, timeout);
}
