...
 // nach Spiegelung:

  // Lichtbrechung
      if (hi.object->getRefractive() && dep < depth) {
        Ray inray;
        inray.origin = hi.position;
        inray.insideObject = hi.object;

        // Winkel bestimmen
        double c1 = -ray.direction.dot(hi.normal)/ray.direction.norm();
        double n = 1.0/hi.object->getRefraction();
        double c2 = sqrt( 1 - n*n*(1-c1*c1) );

        inray.direction = n*ray.direction + (n*c1-c2)*hi.normal;
        inray.direction /= inray.direction.norm();

        // zeigt Strahl ins Objekt? Totalreflektion nicht simuliert
        if (inray.direction.dot(hi.normal) < 0) {
          // Austrittsort und Winkel bestimmen
          HitInfo insidehi = hi.object->isHitBy( inray );
          Ray outray;
          outray.origin = insidehi.position;
          c1 = -inray.direction.dot(-insidehi.normal);
          n = hi.object->getRefraction();
          c2 = sqrt( 1 - n*n*(1-c1*c1) );
          outray.direction = n*inray.direction + (n*c1-c2)*(-insidehi.normal);
          outray.direction /= outray.direction.norm();

          // zeigt Strahl aus Objekt? Totalreflektion nicht simuliert
          Vector3d rgbRefr = scene.getBackground();
          if (outray.direction.dot(insidehi.normal) > 0) {
            // Strahl weiterverfolgen
            rgbRefr = raytrace( scene, outray, dep+1, hi.object );
          }

          // Farbe setzen
          rgbCol += rgbRefr.cwiseProduct(hi.object->getPigment());
        }
      }
    }
    return rgbCol;
  }
