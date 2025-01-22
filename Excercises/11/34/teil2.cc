Vector3d Raytracer::raytrace( const Scene &scene, const Ray &ray, int dep,
                                Object *origin )
  {
    Vector3d rgbCol;
    HitInfo hi;
    for (auto o: scene.objects) {
      if (o.get() == origin) // Erste Objekt?
        continue;

      HitInfo hitest = o->isHitBy( ray );
      if ( hitest.hit
           && (hitest.rayparam < hi.rayparam)
           && (hitest.rayparam >= 0) ) {
        hi = hitest;
      }
    }

    if (!hi.hit) {
      rgbCol = scene.getBackground();
    }
    else {

    ...
