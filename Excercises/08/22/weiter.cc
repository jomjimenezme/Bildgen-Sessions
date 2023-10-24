 void weiter()
  {
    string neu = "";
    double r;
    for (auto z : ableitung)
      switch (z)
        {
          case 'K':
            r = rnd01();
            if (r < 0.2)               // 20%
              neu += 'K';
            else if (r < 0.25)         // 5%
              break; // ε
            else if (r < 0.375)        // 12,5%
              neu += 'B';
            else if (r < 0.5)          // 12,5%
              neu += 'l';
            else if (r < 0.625)        // 12,5%
              neu += "zK";
            else if (r < 0.75)         // 12,5%
              neu += "(K)zK";
            else if (r < 0.875)        // 12,5%
              neu += "[K]zK";
            else                       // 12,5%
              neu += "(K)[K]zK";
            break;
          case 'B':
            r = rnd01();
            if (r < 0.5)
              neu += 'B';
            // ε für r ≥ 0.5
            break;
          default:
            neu += z;
        }
    ableitung = neu;
  }
  
