#include "client.h"
#include "ByteBuffer.h"
#include <stdio.h>

int main() {
    ByteBuffer test;
    test.writeByte(120);
    test.writeInt(1234567);
    test.writeLong(INT64_MAX);
    test.writeVarLong(14235834664);
    test.writeString("test string");

    printf("%d\n", test.readByte());
    printf("%d\n", test.readInt());
    printf("%lld\n", test.readLong());
    printf("%lld\n", test.readVarLong());
    printf("%s\n", test.readString().data());
    
    tcp_connect("localhost", 29077);
}