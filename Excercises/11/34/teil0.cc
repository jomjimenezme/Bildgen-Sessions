 void Raytracer::raytrace( const Scene &scene, Image &image )
  {
 // Ecke und spannende Vektoren der Projektionsebene bestimmen
    Vector3d campos = scene.getCamera().getPosition();
    Vector3d camdir = scene.getCamera().getDirection();
    Vector3d up     = scene.getCamera().getUp();
    double horangle = scene.getCamera().getHorAngle();
    double aspect   = static_cast<double>(image.getWidth())/image.getHeight();

    Vector3d planeright = camdir.cross(up);
    Vector3d planedown  = camdir.cross(planeright);

    camdir     /= camdir.norm();
    planeright /= planeright.norm();
    planedown  /= planedown.norm();

    double pi = M_PI;
    planeright *= tan(horangle * pi / 360.0);
    planedown  *= tan(horangle * pi / 360.0) / aspect;

    Vector3d topleft = campos + camdir - planeright - planedown;

    // Größe des Bildes ermitteln und temporäres Bild erzeugen
    int w, h;

    w = image.getWidth();
    h = image.getHeight();

    ...
