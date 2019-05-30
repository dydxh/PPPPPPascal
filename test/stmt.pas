program stmt;
const
    a = 1;

var
    b, c : integer;

begin
    
    if a = 1 then
    begin
        for d := 1 to 10 do begin
            b := 1;
            while b < 10 do begin
                c := 1;
                repeat
                    a := a + 1;
                until c >= 10;
            end;
        end;
    end
    else begin
        writeln('OvO');
    end;
    
end.