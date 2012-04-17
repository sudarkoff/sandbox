#!/usr/sbin/dtrace -s
#pragma D option quiet

pid$target::_emalloc:entry
/ arg0 > 131072 /
{
    self->inemalloc = 1;
}

pid$target::_emalloc:return
/ arg0 > 131072 /
{
    self->inemalloc = 0;
}

syscall:::entry
/ self->inemalloc /
{
    printf("%s", probefunc);
}
