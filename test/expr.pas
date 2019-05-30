program exprcheck;
CONST
    a = 1;
    b = 2.2;
    c = 'string';
    d = false;

TYPE
    z = record
            a : integer;
            b : integer;
        end;

VAR
    e : integer;
    f : real;
    g : longint;
    h : string;
    i : boolean;
    j : array [-2..2] of integer;
    k : z;

function test : integer;
begin
    test := 3;
end;

begin
    e := a + b;
    g := a - b;
    f := a * b;
    f := a / b;
    i := a = b;
    i := a <> b;
    i := a > b;
    i := a < b;
    h := 'dydxh';
    f := 2.2;
    g := 3;
    i := false;
    e := a and b;
    e := a or b;
    e := a xor b;
    e := a div g;
    e := a mod g;
    e := not g;
    j[-2] := -2;
    j[-1] := -1;
    j[0] := 0;
    j[1] := 1;
    j[2] := 2;

    k.z := (j[-2] + a - g) * 4 / 3 ** 3 + test();

end.