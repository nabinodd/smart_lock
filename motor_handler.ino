void write(int a, int b, int c, int d)
{
    digitalWrite(A, a);
    digitalWrite(B, b);
    digitalWrite(C, c);
    digitalWrite(D, d);
}
void stepF(int x = 1, int d = 900)
{
    for (int i = 0; i < x; i++)
    {
        write(1, 0, 0, 0);
        delayMicroseconds(d);
        write(1, 1, 0, 0);
        delayMicroseconds(d);
        write(0, 1, 0, 0);
        delayMicroseconds(d);
        write(0, 1, 1, 0);
        delayMicroseconds(d);
        write(0, 0, 1, 0);
        delayMicroseconds(d);
        write(0, 0, 1, 1);
        delayMicroseconds(d);
        write(0, 0, 0, 1);
        delayMicroseconds(d);
        write(1, 0, 0, 1);
        delayMicroseconds(d);
    }
}

void stepR(int x = 1, int d = 900)
{
    for (int i = 0; i < x; i++)
    {
        write(0, 0, 0, 1);
        delayMicroseconds(d);
        write(0, 0, 1, 1);
        delayMicroseconds(d);
        write(0, 0, 1, 0);
        delayMicroseconds(d);
        write(0, 1, 1, 0);
        delayMicroseconds(d);
        write(0, 1, 0, 0);
        delayMicroseconds(d);
        write(1, 1, 0, 0);
        delayMicroseconds(d);
        write(1, 0, 0, 0);
        delayMicroseconds(d);
        write(1, 0, 0, 1);
        delayMicroseconds(d);
    }
}