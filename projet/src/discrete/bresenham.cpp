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


#include "lib/common/error_handling.hpp"

#include "discrete/line_discrete.hpp"
#include "bresenham_octant.hpp"

namespace cpe
{



/** Compute Bresenham algorithm in the first octant */
void bresenham_first_octant(ivec2 const& p0,ivec2 const& p1,line_discrete& line,int const octant)
{

    // TO DO

    // Completez l'algorithme de Bresenham
    // init dx dy
    int const dx = p1.x()-p0.x();
    int const dy = p1.y()-p0.y();
    // erreur <-  -dx
    int e = -dx;
    // m      <- 2 dy
    int m = 2*dy;
    int y = p0.y();
    // Pour tous les pixels du segment
    for(int x=p0.x(); x<=p1.x(); ++x)
    {
        //   Ajouter point courant (x,y) au segment

        line.push_back(symmetry_octant_inverse({x,y},octant));

        //   Ajouter m a l'erreur
        e += m;
        //   Si erreur >= 0
        if (e>=0)
        {
            //      Incrementer y
            y++;
            //      Enlever 2 dx a l'erreur
            e -= 2*dx;
        }
    }
}

void bresenham(ivec2 const& p0,ivec2 const& p1,line_discrete& line)
{
    // Compute the corresponding p0,p1 in the symmetrical first octant
    int const octant = octant_number(p0,p1);
    ivec2 const p0_octant = symmetry_octant(p0,octant);
    ivec2 const p1_octant = symmetry_octant(p1,octant);

    bresenham_first_octant(p0_octant,p1_octant,line,octant);

    // Sanity check ensuring the discrete line is correctly computed
    ASSERT_CPE(line.size()>0,"Incorrect size of line in Bresenham ( line.size()="+std::to_string(line.size())+" )");
    ASSERT_CPE(line[0]==p0,"Incorrect first point in Bresenham ( line[0]=("+to_string(line[0])+") ; p0=("+to_string(p0)+") )");
    ASSERT_CPE(line[line.size()-1]==p1,"Incorrect last point in Bresenham ( line[end]=("+to_string(line[line.size()-1])+") ; p1=("+to_string(p1)+") )");
}




}
