void maleMich(Drawing& pic) const
  {
    DPoint2D pos = position;
    DPoint2D posneu;
    double winkel(M_PI / 2);
    stack<DPoint2D> posstack;
    stack<double> winkelstack;
    int level = 1;
    
    for (auto z : ableitung)
      switch (z)
        {
          case 'K': // Knospe: kleiner blauer Kreis
            pic.drawCircle(pos, 3, true, DrawColour(95, 95, 255),
                           DrawColour(197, 197, 255));
            break;
          case 'B': // Blüte: drei rote Kreise, zwei klein, einer größer
            pic.drawCircle(pos + DPoint2D(-6, 0), 3, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            pic.drawCircle(pos + DPoint2D(6, 0), 3, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            pic.drawCircle(pos, 5, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            break;
          case 'z': // Zweig: braune Linie, kürzer für tiefere Rekursionsebenen
            posneu
              = pos + 14 * exp(1 / level) * DPoint2D(cos(winkel), sin(winkel));
            pic.drawLine(pos, posneu, DrawColour(164, 70, 8));
            pos = posneu;
            break;
          case 'l': // Blatt: größerer grüner Kreis
            pic.drawCircle(pos, 5, true, DrawColour(12, 180, 12),
                           DrawColour(197, 237, 197));
            break;
          case '(': // Neigung nach Links
            posstack.push(pos);
            winkelstack.push(winkel);
            winkel += M_PI / 8 * pow(0.9, level - 1);
            ++level;
            break;
          case '[': // Neigung nach Rechts
            posstack.push(pos);
            winkelstack.push(winkel);
            winkel -= M_PI / 8 * pow(0.9, level - 1);
            ++level;
            break;
          case ')':
          case ']':
            pos = posstack.top();
            posstack.pop();
            winkel = winkelstack.top();
            winkelstack.pop();
            --level;
        }
    cout << "Gemalt: " << ableitung << endl;
  }
