    ...
      // Ambienter Anteil
      rgbCol = scene.getAmbient().cwiseProduct(hi.object->getAmbient());

      // Lichtbeitrag aller Lichtquellen ermitteln
      VisibleInfo vi;
      for (auto l: scene.lights) {
        vi = l->visibleFrom( scene, hi, hi.object );

        if (vi.visible) {
          // diffuser und winkelabhängiger Anteil
          Vector3d diffspec(0,0,0);   // <-- kombinierter diffuser und
          //                                 winkelabhängiger Lichtanteil

          double dotNormalLightDir = hi.normal.dot(vi.direction);
          if (dotNormalLightDir > 0) {
            // diffuser Anteil
            diffspec = dotNormalLightDir*hi.object->getDiffuse();

            // winkelabhängiger Anteil
            Vector3d tocam = -ray.direction/ray.direction.norm();
            Vector3d lightreflectdir
              = 2*dotNormalLightDir*hi.normal-vi.direction;
            double dotTocamLightreflect = tocam.dot(lightreflectdir);
            if (dotTocamLightreflect > 0)
              diffspec += pow(dotTocamLightreflect,
                              hi.object->getSpecularityExponent())
                *hi.object->getSpecular();
          }

          // zusammen zur Farbe
          rgbCol += l->getAttenuation(vi.distance)  // is this g?
            *l->getIntensity().cwiseProduct(diffspec);
        }
      }

    ... was fehlt noch?
