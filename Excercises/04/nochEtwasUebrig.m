function [ue, myIn, myOut] = nochEtwasUebrig(z, n, myIn, myOut)
    if n > 0
        myIn = max(myIn, z / n);
        ue = (myIn <= myOut);
    elseif n < 0
        myOut = min(myOut, z / n);
        ue = (myIn <= myOut);
    else
        ue = (z <= 0);
    end
end
