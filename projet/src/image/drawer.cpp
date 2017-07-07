/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "drawer.hpp"

#include "discrete/ivec2.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "discrete/bresenham.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image.hpp"
#include "image_zbuffer.hpp"
#include "color.hpp"
#include "lib/3d/vec2.hpp"
#include "texture.hpp"

namespace cpe
{


void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c)
{

    // TO DO

    // Affichez une ligne sur l'image
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Pour tous les pixel p du segment
    //   Affecter im(p) a la couleur c

    line_discrete line;
    bresenham(p0,p1,line);
    for(ivec2 const& p : line)
    {
        im(p.x(),p.y()) = c;
    }



}

void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c0,color const& c1)
{


    // TO DO

    // Affichez une ligne de couleur variable entre p0 de couleur c0, et p1 de couleur c1
    //
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Construire une structure line_interpolation_parameter
    //   a partir de la ligne discrete
    //
    // Pour toute les pixels de la ligne a l'indice k
    //    p <- position a l'indice k
    //    alpha <- parametre d'interpolation a l'indice k
    //    Calculer couleur courante en fonction de alpha, c0, et c1
    //    Appliquer la couleur courante a im(p)

    line_discrete line;
    bresenham(p0,p1,line);
    line_interpolation_parameter interpolation(line);
    for(int k=0;k<line.size();++k)
    {
        ivec2 p = line[k];
        float a = interpolation[k];
        color c = (1-a)*c0 + a*c1;
        im(p) = c;
    }
}

void draw_line(image& im, image_zbuffer& zbuffer,ivec2 const& p0,ivec2 const& p1,
               color const& c0,color const& c1, float const& z0, float const& z1)
{
    line_discrete line;
    bresenham(p0,p1,line);
    line_interpolation_parameter interpolation(line);
    for(int k=0;k<line.size();++k)
    {
        ivec2 p = line[k];
        float a = interpolation[k];
        color c = (1-a)*c0 + a*c1;
        float z = (1-a)*z0 + a*z1;
        draw_point(im, zbuffer, p, z,c);
    }

}

// Avec texture
void draw_line(image& im, image_zbuffer& zbuffer,ivec2 const& p0,ivec2 const& p1,
               color const& c0,color const& c1, float const& z0, float const& z1,
               vec2 const& t0, vec2 const& t1,  const texture &text)
{
    line_discrete line;
    bresenham(p0,p1,line);
    line_interpolation_parameter interpolation(line);
    for(int k=0;k<line.size();++k)
    {
        ivec2 p = line[k];
        float a = interpolation[k];
        color c = (1-a)*c0 + a*c1;
        float z = (1-a)*z0 + a*z1;
        vec2 t = (1-a)*t0 + a*t1;
        draw_point(im, zbuffer, p, z,c, t, text);
    }

}



void draw_triangle_wireframe(image& im, ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c)
{
    draw_line(im,p0,p1,c);
    draw_line(im,p1,p2,c);
    draw_line(im,p2,p0,c);
}


void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c)
{



    // TO DO

    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   le parametere peut etre definit arbitrairement.
    //   (par exemple triangle_scanline_factory(p0,p1,p2 , c,c,c); )
    triangle_scanline<color> triangle = triangle_scanline_factory(p0,p1,p2 , c,c,c);
    // Pour toutes les valeurs y du scanline
    for (auto const& value : triangle)
    {
        //   pa <- position a gauche
        //int const x = value.first;
        auto const& left = value.second.left;
        ivec2 const& pa = left.coordinate;

        //   pb <- position a droite
        auto const& right = value.second.right;
        ivec2 const& pb = right.coordinate;

        //   Afficher segment entre pa et pb de couleur c
        color const& c = left.parameter;
        draw_line(im,pa,pb,c);

    }

    //
}

void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2)
{

    // TO DO

    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   de parametre (c0,c1,c2)
    //
    // Pour toutes les valeurs y du scanline
    //   pa <- position a gauche
    //   pb <- position a droite
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   Afficher segment entre pa et pb
    //        avec couleur variant entre ca et cb
    //


    triangle_scanline<color> triangle = triangle_scanline_factory(p0, p1, p2, c0, c1, c2);
    for (auto const& value : triangle)
    {
        //   pa <- position a gauche
        //int const x = value.first;
        auto const& left = value.second.left;
        ivec2 const& pa = left.coordinate;

        //   pb <- position a droite
        auto const& right = value.second.right;  // variable composée d'un ivec2 (coordinate) et
                                                 // d'un paramètre de type template (parameter)
        ivec2 const& pb = right.coordinate;

        //   Afficher segment entre pa et pb avec couleur variant entre ca et cb
        color const& ca = left.parameter;
        color const& cb = right.parameter;

        draw_line(im,pa,pb,ca,cb);

    }

}


void draw_point(image& im,image_zbuffer& zbuffer,ivec2 const& p,float const z,color const& c)
{
    if(p.x()<0 || p.x()>=im.Nx())
        return ;
    if(p.y()<0 || p.y()>=im.Ny())
        return ;

    /*************************************
    // TO DO
    /*************************************
    // Si z est compris dans [-1,1] et que z
    //   est plus faible que zbuffer(p)
    //  Alors mettre a jour la couleur et le zbuffer
    */

    if (z > -1.0 && z < 1.0 && z < zbuffer(p))
    {
        im(p) = c;
        zbuffer(p) = z;
    }
}

// Avec texture
void draw_point(image& im, image_zbuffer& zbuffer, ivec2 const& p, float const z, color const& c, vec2 const& t, const texture &text)
{
    if(p.x()<0 || p.x()>=im.Nx())
        return ;
    if(p.y()<0 || p.y()>=im.Ny())
        return ;

    /*************************************
    // TO DO
    /*************************************
    // Si z est compris dans [-1,1] et que z
    //   est plus faible que zbuffer(p)
    //  Alors mettre a jour la couleur et le zbuffer
    */

    if (z > -1.0 && z < 1.0 && z < zbuffer(p))
    {
        im(p) = c * text(t);
        zbuffer(p) = z;
    }
}





void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2)
{


    /*************************************
    // TO DO
    /*************************************
    // Construire scanline avec couleur
    // Construire scanline avec profondeur
    // Parcourir les deux scanline (utilisez les iterateurs)
    //   pa <- point gauche
    //   pb <- point droit
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   za <- profondeur a gauche
    //   zb <- profondeur a droite
    //
    //   affiche ligne entre pa et pb
    //     de couleur variant entre ca et cb
    //     de pronfondeur variant entre za et zb
    */

    triangle_scanline<color> triangle = triangle_scanline_factory(p0, p1, p2, c0, c1, c2);
    triangle_scanline<float> triangleProfondeur = triangle_scanline_factory(p0, p1, p2, z0, z1, z2);

    auto it = triangle.begin();
    auto it_end = triangle.end();
    auto it_profondeur = triangleProfondeur.begin();
    auto it_profondeur_end = triangleProfondeur.end();
    for(  ; it != it_end && it_profondeur != it_profondeur_end; ++it , ++it_profondeur)
    {
        //   pa <- position a gauche
        //int const x = value.first;
        auto const& left = it->second.left;
        ivec2 const& pa = left.coordinate;

        //   pb <- position a droite
        auto const& right = it->second.right;
        ivec2 const& pb = right.coordinate;

        //   Afficher segment entre pa et pb avec couleur variant entre ca et cb et pronfondeur variant entre za et zb
        color const& ca = left.parameter;
        color const& cb = right.parameter;

        // profondeur
        auto const& left_profondeur = it_profondeur->second.left;
        //   pb <- position a droite
        auto const& right_profondeur = it_profondeur->second.right;

        float const& za = left_profondeur.parameter;
        float const& zb = right_profondeur.parameter;

        draw_line(im, zbuffer, pa, pb, ca, cb, za, zb);

    }



}

// avec texture
void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2,
                   vec2 const& t0,vec2 const& t1,vec2 const& t2, const texture &text)
{


    /*************************************
    // TO DO
    /*************************************
    // Construire scanline avec couleur
    // Construire scanline avec profondeur
    // Parcourir les deux scanline (utilisez les iterateurs)
    //   pa <- point gauche
    //   pb <- point droit
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   za <- profondeur a gauche
    //   zb <- profondeur a droite
    //
    //   affiche ligne entre pa et pb
    //     de couleur variant entre ca et cb
    //     de pronfondeur variant entre za et zb
    */

    triangle_scanline<color> triangle = triangle_scanline_factory(p0, p1, p2, c0, c1, c2);
    triangle_scanline<float> triangleProfondeur = triangle_scanline_factory(p0, p1, p2, z0, z1, z2);
    triangle_scanline<vec2>  triangleTexture = triangle_scanline_factory(p0, p1, p2, t0, t1, t2);

    auto it = triangle.begin();
    auto it_end = triangle.end();
    auto it_profondeur = triangleProfondeur.begin();
    auto it_profondeur_end = triangleProfondeur.end();
    auto it_texture = triangleTexture.begin();
    auto it_texture_end = triangleTexture.end();
    for(  ; it != it_end && it_profondeur != it_profondeur_end && it_texture != it_texture_end;
          ++it , ++it_profondeur, ++it_texture)
    {
        //   pa <- position a gauche
        //int const x = value.first;
        auto const& left = it->second.left;
        ivec2 const& pa = left.coordinate;

        //   pb <- position a droite
        auto const& right = it->second.right;
        ivec2 const& pb = right.coordinate;

        //   Afficher segment entre pa et pb avec couleur variant entre ca et cb et pronfondeur variant entre za et zb
        color const& ca = left.parameter;
        color const& cb = right.parameter;

        // profondeur
        auto const& left_profondeur = it_profondeur->second.left;
        //   pb <- position a droite
        auto const& right_profondeur = it_profondeur->second.right;

        float const& za = left_profondeur.parameter;
        float const& zb = right_profondeur.parameter;

        // texture
        auto const& left_texture = it_texture->second.left;
        //   pb <- position a droite
        auto const& right_texture = it_texture->second.right;

        vec2 const& ta = left_texture.parameter;
        vec2 const& tb = right_texture.parameter;

        draw_line(im, zbuffer, pa, pb, ca, cb, za, zb, ta, tb, text);

    }



}




}
