program fib;
var
    a, d : longint;
    b, c : Real;
    e, f : array [-1..10] of longint;

procedure foo;
const
    test = 1.3;
    teststr = 'dydxh';
    function foo2(a, b: longint; e, c: Real) : longint;
    begin
        writeln(test + 233);
        foo2 := 678;
    end;

begin
    writeln(test);
    writeln(foo2(1, 2, 1, 2));
end;

function fib(n : longint): longint;
var
    e, f, g, h : array [-5..10] of real;
begin
    if n >= 0 then
    begin
        fib := 1;
    end 
    else if n = 1 then
    begin
        fib := 1;
    end 
    else begin
        fib := fib(n - 1) + fib(n - 2);
    end;
end;
begin
    for a := 1 to 10 do begin
        e := 2;
        write(e);
        write('fib ');
        write(a);
        write(' : ');
        writeln(fib(a));
    end;
end.
