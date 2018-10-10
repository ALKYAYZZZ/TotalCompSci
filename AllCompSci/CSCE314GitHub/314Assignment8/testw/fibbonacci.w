var x = 4;
var count = 0;
var a = 0;
var b = 1;
var c = 0;
while(count <= x)
{
c = a + b;
b = a;
a = c;
count = count + 1;
}
x = c;
print x;