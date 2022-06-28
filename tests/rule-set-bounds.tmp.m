  const
    MAX : 64;
  type
    R : 1..MAX;
  var
    r1 : R;

  ruleset r2 : R do
    ruleset r3 : 1..r2 do
      ruleset r4 : scalarset(r3) do
        ruleset r5 := 1 to MAX by r3 do
          rule "example rule" 
              ((r1 + r2 + r3 + r4 + r5) >= MAX)
            ==>
              r1 := ((r1 + 1) % MAX) + 1;
          end;
        endruleset;
      endruleset;
    endruleset;
  endruleset;

  ruleset r2 : 1..r1 do
    ruleset r3 := 1 to r1 by r2 do
      rule "example rule" 
          ((r1 + r2 + r3) >= MAX)
        ==>
          r1 := ((r1 + 1) % MAX) + 1;
      end;
    endruleset;
  endruleset;
