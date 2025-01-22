    ...
    // Spiegelung
      if (hi.object->getReflective() && dep < depth) {
        Vector3d tocam = -ray.direction/ray.direction.norm();
        Ray reflray;
        reflray.origin = hi.position;
        reflray.direction = 2*tocam.dot(hi.normal)*hi.normal-tocam;
        Vector3d rgbRefl = raytrace( scene, reflray, dep+1, hi.object );
        rgbCol += rgbRefl.cwiseProduct(hi.object->getReflection());
      }

    }

    //Aufagebe 2 (Optional)
    return rgbCol;
  }
