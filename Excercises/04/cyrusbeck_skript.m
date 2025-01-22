% Daten für diese Aufgabe
umin = 2;
umax = 8;
vmin = 1;
vmax = 5;
linien = [ 1  2 10  4;         % zeilenweise P₁.x, P₁.y, P₂.x, P₂.y
           3  3  6  0;
           7  0 10  2;
           5  6 11  4 ];
m = size(linien, 1);

% allgemeiner Algorithmus
for j = 1 : m                 % alle Linien
    p1 = linien(j, 1:2)';
    p2 = linien(j, 3:4)';
    fprintf("Linie [(%g, %g)–(%g, %g)]: ", p1(1), p1(2), p2(1), p2(2));
    delta = p2 - p1;
    if all(p1 == p2)
        fprintf("entartete Linie (Punkt)\n");
    else
        myIn = 0;
        myOut = 1;
        [ue, myIn, myOut] = nochEtwasUebrig(p1(1) - umax, -delta(1), myIn, myOut);
        if ue
            [ue, myIn, myOut] = nochEtwasUebrig(p1(2) - vmax, -delta(2), myIn, myOut);
            if ue
                [ue, myIn, myOut] = nochEtwasUebrig(umin - p1(1), delta(1), myIn, myOut);
                if ue
                    [ue, myIn, myOut] = nochEtwasUebrig(vmin - p1(2), delta(2), myIn, myOut);
                    if ue
                        if myOut < 1
                            p2 = p1 + myOut * delta;
                        end
                        if myIn > 0
                            p1 = p1 + myIn * delta;
                        end
                        fprintf("wird gekappt zu: [(%g, %g)–(%g, %g)]\n", p1(1), p1(2), p2(1), p2(2));
                    else
                        fprintf("nicht sichtbar\n");
                    end
                else
                    fprintf("nicht sichtbar\n");
                end
            else
                fprintf("nicht sichtbar\n");
            end
        else
            fprintf("nicht sichtbar\n");
        end
    end
end

% Placeholder for the `nochEtwasUebrig` function
function [ue, myIn, myOut] = nochEtwasUebrig(val, delta, myIn, myOut)
    % Logic for nochEtwasUebrig should be implemented here.
    % Replace this placeholder with the actual implementation.
    ue = true; % Example placeholder output
end
