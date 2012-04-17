unsigned int flip(unsigned int i)
{
 unsigned int j=((i&0x0000ffff)<<16)|((i&0xffff0000)>>16);
 return ((j&0x00ff00ff)<<8)|((j&0xff00ff00)>>8);
}
