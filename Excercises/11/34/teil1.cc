    ...

    vector<vector<Vector3d>> tmpImage;
    tmpImage.resize( h );
    for (int y = 0; y < h; y++) {
      tmpImage[y].resize( w, Vector3d(0, 0, 0) );
    }

    // Pixelmaße
    double w1 = 1.0 / w;
    double h1 = 1.0 / h;
    double w1_halbe = 0.5 * w1;
    double h1_halbe = 0.5 * h1;

    // alle Punkte auf der Projektionsebene erzeugen und Strahlen verfolgen
    Ray currentray;
    currentray.origin = campos;
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        currentray.direction = topleft - campos
                             + 2 * planeright * ( x * w1 + w1_halbe )
                             + 2 * planedown  * ( y * h1 + h1_halbe );

        tmpImage[y][x] +=  raytrace( scene, currentray, 1 );
        // Farben im resultierenden Bild setzen + Beobachtung des Bildaufbaus
        Vector3d col;

        col = tmpImage[y][x] ;
        clampCol(col);
        image.setPixel( x, y, col );
      }
    }

}
